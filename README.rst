LATA (Loongson Architecture Translator for ARM)
===========

LATA（Loongson Architecture Translator for ARM）即龙芯ARM架构转译器
，是 一个面向 LoongArch 架构的高性能用户级二进制翻译器，用于在龙芯（龙架构）
系统上高效地运行ARM应用程序。

项目背景
========

在LoongArch架构生态建设过程中，运行已有的ARM程序存在兼容性和性能瓶颈，
原生QEMU等模拟器在性能和兼容性上并不能完全满足需求。因此，我们在QEMU 8.1.5
的基础上进行了二次开发，通过引入TU模块、指令融合以及其他针对性优化，减少了
指令翻译和执行的开销，努力实现“更快、更稳定、更兼容”的目标。

项目结构
========
下面是本项目的主要目录结构:

.. code-block:: text

   lata
   ├── ...
   ├── latabuild/                           # 编译脚本
   ├── target/
   │   └── arm/
   │       └── lata/
   │           └── include/                 # 头文件汇总
   │               └── ir1.h  
   │               └── ir2.h
   │               └── ...
   │           └── ir1/
   │               └── decode_ir1_a64.c.inc # arm 指令解码 IR1
   │               └── ir1.c                # IR1 指令类型处理    
   │               └── ...                
   │           └── ir2/
   │               └── ir2.c                # IR2：LA 指令 IR 表示
   │               └── la-append.c          # 由脚本生成的 IR 处理函数
   │               └── ir2-assemble.c       # IR2 辅助函数
   │               └── ...                    
   │           └── optimization/
   │               └── insts-pattern.c      # 语义级指令组合优化翻译
   │               └── tu.c                 # TU 翻译单元优化
   │               └── clear-high.c         # 高位清除优化
   │               └── ...
   │           └── translator/              # 翻译函数
   │               └── translate-a64.c      # 逻辑运算指令翻译函数
   │               └── macro-helper.c       # 指令翻译操作辅助函数
   │               └── ...
   │           └── mem.c                    # 内存操作辅助函数
   │           └── lata.c        
   │           └── ...
   ├── ...
   └── README.rst                           # 本文档

编译
=====================

.. code-block:: shell

   # 安装编译依赖
   apt install -y \
       git \
       ninja-build \
       libssl-dev \
       libc6 \
       gcc \
       g++ \
       pkg-config \
       libglib2.0-dev \
       libdrm-dev \
       lsb-release \
       make \
       python3-setuptools \
       capstone-tool

   # 克隆源码
   git clone --depth=1 --recursive https://github.com/lata-opensource/lata
   cd lata
   # 脚本构建(开优化性能测试)
   ./latabuild/lata-opt.sh
   # or(优化调试)
   ./latabuild/lata-debug.sh


未来规划（TODO）
===============

项目未来的优化与完善方向包括但不限于：

- [ ] 优化翻译器性能，以提高翻译器的可用性。
- [ ] 动态链接调试，glibc测试完备。
- [ ] 支持更复杂的ARM指令集扩展。
- [ ] 支持bionic库， 实现在安卓平台上运行应用程序。
- [ ] 维护更详细的文档与使用指南。

欢迎大家通过 Issues 等方式讨论新特性需求、Bug 反馈以及优化思路。我们期待更
多开发者与社区力量的加入，一同推进 LoongArch  生态建设！

许可证
======

本项目基于 QEMU 源代码进行二次开发，原始项目遵循 GNU 通用公共许可证第 2 版
（GNU General Public License, version 2，简称 GPLv2）发布。

因此，本项目同样遵循 GPLv2 协议。

致谢
====

特别鸣谢 QEMU 项目及开发者，他们的开源成果为本项目提供了宝贵
的参考与支持。

------------

如有任何问题或建议，欢迎通过 Issue 与我们交流！
