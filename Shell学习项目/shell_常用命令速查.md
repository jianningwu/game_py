# Shell 常用命令速查手册

> 按分类整理的 Linux/Shell 常用命令，含用途和常用选项。
> 更新日期：2026年5月

---

## 一、文件与目录操作

| 命令 | 用途 | 常用选项/示例 |
|------|------|-------------|
| `ls` | 列出目录内容 | `-l` 详细, `-a` 含隐藏, `-h` 人类可读大小 |
| `cd` | 切换目录 | `cd ~` 回家, `cd -` 回上一目录 |
| `pwd` | 打印当前目录 | `pwd` |
| `mkdir` | 创建目录 | `-p` 递归创建 `mkdir -p a/b/c` |
| `rmdir` | 删除空目录 | - |
| `rm` | 删除文件/目录 | `-r` 递归, `-f` 强制, `rm -rf` 危险！ |
| `cp` | 复制 | `-r` 递归, `-p` 保留属性, `cp src dst` |
| `mv` | 移动/重命名 | `mv old new` |
| `touch` | 创建空文件/更新时间戳 | `touch file.txt` |
| `ln` | 创建链接 | `-s` 软链接 `ln -s target link` |
| `stat` | 查看文件详细信息 | `stat file` |
| `file` | 判断文件类型 | `file unknown` |
| `tree` | 树形显示目录结构 | `tree -L 2` 限制深度 |
| `du` | 磁盘使用量 | `-h` 可读, `-s` 汇总, `du -sh *` |
| `df` | 磁盘剩余空间 | `-h` 可读 `df -h` |

---

## 二、文本查看与处理

| 命令 | 用途 | 常用选项/示例 |
|------|------|-------------|
| `cat` | 连接并输出文件 | `-n` 编号, `cat a.txt b.txt` |
| `less` | 分页查看（可上下翻） | `/pattern` 搜索, `q` 退出 |
| `more` | 分页查看（仅下翻） | - |
| `head` | 查看文件头 N 行 | `-n N` `head -n 10 file` |
| `tail` | 查看文件尾 N 行 | `-n N`, `-f` 实时跟踪 `tail -f log` |
| `wc` | 统计 | `-l` 行, `-w` 单词, `-c` 字节 |
| `sort` | 排序 | `-n` 数值, `-r` 逆序, `-u` 去重, `-k N` 按第N列 |
| `uniq` | 去重（需先排序） | `-c` 计数, `-d` 只显示重复项 |
| `cut` | 按列截取 | `-d` 分隔符, `-f` 字段 `cut -d',' -f1` |
| `tr` | 字符转换/删除 | `tr 'a-z' 'A-Z'`, `tr -d '\r'` |
| `diff` | 逐行比较文件 | `-u` unified 格式 `diff -u old new` |
| `patch` | 应用补丁 | `patch < diff.patch` |
| `tee` | 输出到屏幕同时写文件 | `cmd | tee log.txt` |

---

## 三、文本处理三剑客

### grep — 文本搜索

| 选项 | 用途 | 示例 |
|------|------|------|
| `-i` | 忽略大小写 | `grep -i "error" log` |
| `-v` | 反向匹配(不包含) | `grep -v "#" config` |
| `-c` | 计数 | `grep -c "200" access.log` |
| `-n` | 显示行号 | `grep -n "TODO" *.py` |
| `-r` | 递归搜索目录 | `grep -r "import" src/` |
| `-l` | 只列文件名 | `grep -rl "pattern" /dir` |
| `-E` | 扩展正则 | `grep -E "error|warn" log` |
| `-o` | 只输出匹配部分 | `grep -o '[0-9]\+' file` |
| `-A/B/C N` | 上下文行 | `grep -C 3 "key" file` |

### sed — 流编辑器

| 操作 | 示例 | 说明 |
|------|------|------|
| 替换 | `sed 's/old/new/g'` | g=全局, 去掉g只替换第一个 |
| 替换(原位) | `sed -i 's/old/new/g' file` | 直接修改文件 |
| 删除行 | `sed '3d'` 或 `sed '/pattern/d'` | 删除第3行或匹配行 |
| 打印行 | `sed -n '5,10p'` | 打印5-10行 |
| 插入 | `sed '3i\text'` | 第3行前插入 |
| 追加 | `sed '3a\text'` | 第3行后追加 |
| 提取 | `sed -n 's/.*\(pattern\).*/\1/p'` | 提取匹配内容 |

### awk — 数据处理

| 操作 | 示例 | 说明 |
|------|------|------|
| 按列打印 | `awk '{print $1, $3}'` | 空格分隔，打印第1和3列 |
| 指定分隔符 | `awk -F',' '{print $2}'` | 逗号分隔 |
| 条件过滤 | `awk '$3 > 90'` | 第3列大于90的行 |
| 求和 | `awk '{sum+=$2} END{print sum}'` | - |
| 计数 | `awk 'END{print NR}'` | NR=行号 |
| 格式化输出 | `awk '{printf "%-10s %d\n", $1, $2}'` | - |

---

## 四、查找与定位

| 命令 | 用途 | 常用选项/示例 |
|------|------|-------------|
| `find` | 查找文件 | `find /dir -name "*.txt" -type f` |
| | | `-mtime -7` 7天内修改 |
| | | `-size +1M` 大于1MB |
| | | `-exec cmd {} \;` 对每个结果执行命令 |
| `which` | 查找命令位置 | `which python` |
| `whereis` | 查找命令/源码/man | `whereis bash` |
| `locate` | 快速文件查找(需 updatedb) | `locate *.conf` |

---

## 五、进程与系统管理

| 命令 | 用途 | 常用选项/示例 |
|------|------|-------------|
| `ps` | 查看进程 | `ps aux` (BSD), `ps -ef` (Unix) |
| `top` / `htop` | 动态进程监控 | `htop` 更直观 |
| `kill` | 发送信号 | `kill -9 PID` (强制), `kill -15 PID` (优雅) |
| `killall` | 按名称杀进程 | `killall -9 name` |
| `pkill` | 按模式杀进程 | `pkill -f "python app"` |
| `jobs` | 查看后台作业 | `jobs -l` |
| `fg` / `bg` | 前后台切换 | `fg %1` |
| `nohup` | 忽略挂断信号 | `nohup cmd &` |
| `nice` | 调整优先级 | `-20`最高, `19`最低 |
| `lsof` | 列出打开的文件 | `lsof -i :8080` 查看端口占用 |
| `strace` | 跟踪系统调用 | `strace -p PID` |
| `systemctl` | systemd 服务管理 | `start/stop/restart/status/enable` |
| `journalctl` | 查看 systemd 日志 | `journalctl -u service -f` |
| `vmstat` | 虚拟内存统计 | `vmstat 1` |
| `free` | 内存使用 | `-h` 可读 `free -h` |
| `uptime` | 系统运行时间 | - |

---

## 六、网络

| 命令 | 用途 | 常用选项/示例 |
|------|------|-------------|
| `curl` | HTTP 客户端 | `-X POST`, `-H "Header"`, `-d "body"` |
| `wget` | 文件下载 | `-c` 断点续传, `-r` 递归 |
| `ping` | 测试连通性 | `ping -c 4 host` |
| `ss` | socket 统计(替代 netstat) | `-tlnp` 查看监听端口 |
| `nc` (netcat) | 网络工具 | `nc -l 8080` 监听, `nc host port` |
| `scp` | 远程拷贝 | `scp file user@host:/path` |
| `rsync` | 同步文件 | `-avz` 归档/详细/压缩 |
| `ssh` | 远程登录 | `ssh user@host`, `-p port` |
| `dig` | DNS 查询 | `dig domain` |
| `iptables` | 防火墙规则 | - |

---

## 七、压缩与归档

| 命令 | 用途 | 示例 |
|------|------|------|
| `tar` | 打包/解包 | `tar -czf out.tar.gz dir/` 压缩 |
| | | `tar -xzf file.tar.gz` 解压 |
| | | `-c` 创建, `-x` 解压, `-z` gzip, `-j` bzip2, `-f` 文件 |
| `gzip` | 压缩 | `gzip file`, `gunzip file.gz` |
| `zip` | ZIP 压缩 | `zip -r out.zip dir/` |
| `unzip` | ZIP 解压 | `unzip file.zip` |

---

## 八、权限与用户

| 命令 | 用途 | 示例 |
|------|------|------|
| `chmod` | 修改权限 | `chmod +x script`, `chmod 755 file` |
| `chown` | 修改所有者 | `chown user:group file` |
| `sudo` | 以 root 执行 | `sudo cmd` |
| `su` | 切换用户 | `su - user` |
| `whoami` | 当前用户 | `whoami` |
| `id` | 用户/组信息 | `id` |

---

## 九、Shell 变量与语法速查

### 变量操作

```bash
${var}          使用变量
${var:-default} 未定义时返回默认值
${var:=value}   未定义时赋值并返回
${var:+alt}     已定义时返回 alt
${var:?err}     未定义时报错
${#var}         字符串长度
${var#pattern}  删除最短前缀匹配
${var##pattern} 删除最长前缀匹配
${var%pattern}  删除最短后缀匹配
${var%%pattern} 删除最长后缀匹配
${var/old/new}  替换第一个匹配
${var//old/new} 替换所有匹配
```

### 条件判断

```bash
# 文件判断
[[ -f file ]]   是否为普通文件
[[ -d dir ]]    是否为目录
[[ -e path ]]   路径是否存在
[[ -r file ]]   是否可读
[[ -w file ]]   是否可写
[[ -x file ]]   是否可执行
[[ -s file ]]   文件大小 > 0

# 字符串判断
[[ -z str ]]    为空
[[ -n str ]]    非空
[[ str1 == str2 ]]  相等
[[ str1 != str2 ]]  不等
[[ str =~ regex ]]  正则匹配

# 数值判断
[[ n1 -eq n2 ]] 等于
[[ n1 -ne n2 ]] 不等于
[[ n1 -gt n2 ]] 大于
[[ n1 -lt n2 ]] 小于
[[ n1 -ge n2 ]] 大于等于
[[ n1 -le n2 ]] 小于等于
```

### 特殊变量

| 变量 | 含义 |
|------|------|
| `$0` | 脚本名称 |
| `$1..$9` | 位置参数 |
| `$#` | 参数个数 |
| `$@` | 所有参数(独立) |
| `$*` | 所有参数(合并) |
| `$?` | 上条命令退出码 |
| `$$` | 当前 PID |
| `$!` | 最后后台 PID |

### 重定向

| 符号 | 含义 |
|------|------|
| `cmd > file` | stdout 覆盖写入文件 |
| `cmd >> file` | stdout 追加写入文件 |
| `cmd 2> file` | stderr 写入文件 |
| `cmd &> file` | stdout+stderr 写入文件 |
| `cmd < file` | 从文件读取 stdin |
| `cmd1 | cmd2` | 管道：cmd1 的 stdout -> cmd2 的 stdin |
