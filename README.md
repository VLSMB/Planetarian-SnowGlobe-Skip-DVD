# Planetarian-SnowGlobe-Skip-DVD
在不对SiglusEngine进行任何修改的情况下跨过Planetarian SnowGlobe的DVD验证

# 使用方法：
直接把version.dll与SiglusEngine.exe放在一起就可以了

# 代码思路(dllmain.cpp)
Hook点：kernel32.dll的InterlockedDecrement
将RVA 0x165F4E 的 call 0x002812F0 换成 5字节nop，即可跳过DVD验证
先获取InterlockedDecrement的地址，将函数最开始5字节换成jmp指令，跳转到HookDll中，实现功能
跳转时栈中第一个数据即为进程调用InterlockedDecrement的返回值，利用先前获取此处的RVA计算进程基址，再利用call 0x002812F0的RVA找到地址，将其换成nop，最后再把InterlockedDecrement函数的前5字节恢复

# 参考资料：
https://github.com/strivexjun/AheadLib-x86-x64
https://www.bilibili.com/video/BV1ba411Q7Tc?spm_id_from=333.999.0.0
