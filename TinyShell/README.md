# Tiny-shell: A bash-like shell which is really simple

这是学习Unix编程的一个极简shell。通过构建一个简单的shell，能够对shell的工作原理进行一些了解。主要有：

- [x] ~~重定向~~
- [x] ~~流水线~~
- [x] ~~前台信号处理~~
- [ ] 进程组
- [ ] 后台进程
- [ ] 作业控制

以后会逐步更新~

`tsh1`: 构建一个极简shell, 但是为了体现流程省略了许多错误检查，实际工作中应写好错误检查。

`make pipeline`: 具有流水线处理的shell, 且修复了之前必须按特定顺序处理重定向的要求。
现在诸如`sort < f1 > f2` 和`sort > f2 < f1`都能成功处理。

`make tsh2`: 具有信号处理功能的shell，按\<C-c\>不会退出shell。

`make tsh3`: tsh2的升级版，利用setlongjmp来处理\<C-c\>。
