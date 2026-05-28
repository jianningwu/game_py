#!/usr/bin/env python3
"""
================================================================================
脚本功能：生成 Linux 学习路线与知识体系图表
- 图1: Linux 学习路线阶段图
- 图2: Linux 文件系统层次结构 (FHS) 树形图
- 图3: Linux 启动流程图
- 图4: Linux 发行版家族树
- 图5: 权限模型说明图
================================================================================
"""
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
from matplotlib.patches import FancyBboxPatch, FancyArrowPatch
import numpy as np

plt.rcParams['font.family'] = ['Microsoft YaHei', 'SimHei', 'sans-serif']
plt.rcParams['axes.unicode_minus'] = False
OUT = '.'

def box(ax, x, y, w, h, text, color='#3498db', fs=9, tc='white'):
    r = FancyBboxPatch((x-w/2, y-h/2), w, h, boxstyle="round,pad=0.1",
                       fc=color, ec='#2c3e50', lw=1.2, alpha=0.92)
    ax.add_patch(r)
    ax.text(x, y, text, ha='center', va='center', fontsize=fs, color=tc, fontweight='bold')

def arrow(ax, x1, y1, x2, y2, label='', color='#555'):
    ax.annotate('', xy=(x2, y2), xytext=(x1, y1),
                arrowprops=dict(arrowstyle='->', color=color, lw=2))
    if label:
        ax.text((x1+x2)/2+0.1, (y1+y2)/2-0.2, label, fontsize=8, color='#e74c3c', fontweight='bold')

def save(fig, name):
    path = f'{OUT}/{name}'
    fig.savefig(path, dpi=150, bbox_inches='tight', facecolor='white')
    plt.close(fig)
    print(f'  [OK] {path}')

# ==================================================================
# 图1: 学习路线阶段图
# ==================================================================

def chart_roadmap():
    fig, ax = plt.subplots(figsize=(14, 6))
    ax.set_xlim(0, 14); ax.set_ylim(0, 8); ax.axis('off')
    ax.set_title('Linux 学习路线图（6个阶段）', fontsize=16, fontweight='bold', pad=15)

    stages = [
        (1.5, '入门基础\n1-2周', '#3498db', '基本命令\n文件操作\nvim编辑器'),
        (3.5, '系统管理\n2-4周', '#2ecc71', '用户权限\n进程管理\n磁盘/包管理'),
        (6.0, '服务搭建\n3-6周', '#f39c12', 'Nginx/MySQL\n防火墙/DNS\nDocker'),
        (8.5, '自动化\n4-8周', '#e74c3c', 'Shell脚本\nAnsible\n定时任务'),
        (11.0, '性能安全\n持续', '#9b59b6', '性能监控\n安全加固\n故障排查'),
        (13.0, '内核原理\n持续', '#1abc9c', '内存管理\n进程调度\n网络协议栈'),
    ]

    for x, title, color, desc in stages:
        box(ax, x, 5.5, 1.8, 1.2, title, color, fs=9)
        ax.text(x, 4.2, desc, ha='center', fontsize=7.5, color='#555',
                bbox=dict(boxstyle='round', fc='#f8f9fa', alpha=0.8))

    # Connecting arrows
    for i in range(len(stages)-1):
        ax.annotate('', xy=(stages[i+1][0]-1.0, 5.5), xytext=(stages[i][0]+1.0, 5.5),
                    arrowprops=dict(arrowstyle='->', color='#555', lw=2.5))

    # Target roles
    roles = ['运维助理', '运维工程师', '高级运维', 'DevOps/SRE', '架构师']
    role_x = [2.5, 4.5, 7.2, 10.0, 12.5]
    for rx, role in zip(role_x, roles):
        ax.text(rx, 3.2, f'→ {role}', ha='center', fontsize=9, fontweight='bold', color='#2c3e50')

    # Time axis
    ax.axhline(y=2.2, xmin=0.05, xmax=0.95, color='#ccc', lw=1)
    ax.text(7, 1.8, '时间轴', ha='center', fontsize=9, color='#999')
    for pos, label in [(0.08, '第1月'), (0.25, '第2月'), (0.42, '第3月'), (0.6, '第6月'), (0.85, '持续')]:
        ax.text(pos*14, 2.5, label, ha='center', fontsize=7, color='#aaa')
        ax.plot(pos*14, 2.2, '|', color='#ccc', markersize=5)

    save(fig, 'chart_linux_roadmap.png')

# ==================================================================
# 图2: FHS 树形图
# ==================================================================

def chart_fhs():
    fig, ax = plt.subplots(figsize=(16, 9))
    ax.set_xlim(0, 16); ax.set_ylim(0, 10); ax.axis('off')
    ax.set_title('Linux 文件系统层次结构 (FHS)', fontsize=16, fontweight='bold', pad=15)

    # Root
    box(ax, 8, 9.2, 1.8, 0.7, '/ (根目录)', '#2c3e50', fs=11)

    # Level 1
    dirs_l1 = [
        (1.0, 7.8, '/bin->\n/usr/bin', '#3498db', '基本命令'),
        (2.8, 7.8, '/boot', '#e67e22', '内核+引导'),
        (4.6, 7.8, '/dev', '#95a5a6', '设备文件'),
        (6.4, 7.8, '/etc', '#e74c3c', '配置文件'),
        (8.2, 7.8, '/home', '#2ecc71', '用户家目录'),
        (10.2, 7.8, '/root', '#2ecc71', 'root家'),
        (11.8, 7.8, '/var', '#f39c12', '可变数据'),
        (13.2, 7.8, '/usr', '#9b59b6', '用户程序'),
        (14.5, 7.8, '/proc\n/sys', '#1abc9c', '内核接口'),
    ]
    for x, y, label, color, desc in dirs_l1:
        box(ax, x, y, 1.4, 0.75, label, color, fs=7)
        ax.text(x, y-0.55, desc, ha='center', fontsize=6, color='#555')
        ax.plot([8, x], [8.85, 8.2], '-', color='#aaa', lw=0.8)

    # Level 2 details for /etc
    etc_children = [
        (5.0, 6.3, 'passwd\nshadow', '用户/密码'),
        (5.8, 6.3, 'fstab', '挂载表'),
        (6.4, 6.3, 'hosts\nresolv', '网络'),
        (7.2, 6.3, 'ssh/', 'SSH配置'),
        (7.8, 6.3, 'systemd/', '服务配置'),
    ]
    for x, y, label, desc in etc_children:
        box(ax, x, y, 0.8, 0.6, label, '#e74c3c', fs=6, tc='white')
        ax.text(x, y-0.4, desc, ha='center', fontsize=5.5, color='#555')
        ax.plot([6.4, x], [7.4, 6.6], '-', color='#e74c3c', lw=0.6, alpha=0.4)

    # Level 2 details for /var
    var_children = [
        (10.8, 6.3, 'log/', '日志'),
        (11.5, 6.3, 'spool/', '队列'),
        (12.2, 6.3, 'cache/', '缓存'),
        (12.9, 6.3, 'lib/', '运行时'),
    ]
    for x, y, label, desc in var_children:
        box(ax, x, y, 0.7, 0.55, label, '#f39c12', fs=6, tc='white')
        ax.text(x, y-0.4, desc, ha='center', fontsize=5.5, color='#555')
        ax.plot([11.8, x], [7.4, 6.6], '-', color='#f39c12', lw=0.6, alpha=0.4)

    # Legend
    ax.text(0.5, 0.5, 'FHS = Filesystem Hierarchy Standard (文件系统层次标准)',
            fontsize=10, color='#7f8c8d')

    save(fig, 'chart_linux_fhs.png')

# ==================================================================
# 图3: 启动流程图
# ==================================================================

def chart_boot():
    fig, ax = plt.subplots(figsize=(10, 9))
    ax.set_xlim(0, 10); ax.set_ylim(0, 10); ax.axis('off')
    ax.set_title('Linux 启动流程', fontsize=15, fontweight='bold', pad=15)

    steps = [
        (5, 9.0, '1. BIOS/UEFI\n硬件自检，选择启动设备', '#e74c3c'),
        (5, 7.5, '2. Boot Loader (GRUB2)\n加载内核和initramfs到内存', '#f39c12'),
        (5, 6.0, '3. Kernel 初始化\n硬件驱动/内存管理/进程调度', '#3498db'),
        (5, 4.5, '4. initramfs\n临时根文件系统，加载存储驱动', '#2ecc71'),
        (5, 3.0, '5. systemd/init (PID=1)\n第一个用户态进程', '#9b59b6'),
        (2, 1.5, '6a. multi-user.target\n文本模式(服务器默认)', '#1abc9c'),
        (8, 1.5, '6b. graphical.target\n图形界面(桌面版)', '#1abc9c'),
    ]

    for x, y, label, color in steps:
        box(ax, x, y, 3.0, 0.9, label, color, fs=8)

    # down arrows
    for i in range(4):
        arrow(ax, 5.0, steps[i][1] - 0.45, 5.0, steps[i+1][1] + 0.45, '', '#2c3e50')

    # split to 6a and 6b
    arrow(ax, 4.5, 2.55, 2.5, 1.95, '', '#2c3e50')
    arrow(ax, 5.5, 2.55, 7.5, 1.95, '', '#2c3e50')

    save(fig, 'chart_linux_boot.png')

# ==================================================================
# 图4: 发行版家族树
# ==================================================================

def chart_distros():
    fig, ax = plt.subplots(figsize=(14, 7))
    ax.set_xlim(0, 14); ax.set_ylim(0, 8); ax.axis('off')
    ax.set_title('Linux 发行版家族', fontsize=15, fontweight='bold', pad=15)

    # Kernel
    box(ax, 7, 7.3, 2.5, 0.7, 'Linux Kernel', '#2c3e50', fs=10)

    # 4 major families
    families = [
        (1.8, 5.8, 'Debian 系\napt/dpkg', '#e74c3c'),
        (5.0, 5.8, 'Red Hat 系\ndnf/rpm', '#3498db'),
        (9.0, 5.8, 'SUSE / Arch\nzypper/pacman', '#2ecc71'),
        (12.5, 5.8, '嵌入式\nBuildroot/Yocto', '#9b59b6'),
    ]

    for x, y, label, color in families:
        box(ax, x, y, 2.0, 0.8, label, color, fs=8)
        ax.plot([7, x], [6.95, 6.2], '-', color='#aaa', lw=1)

    # Children for Debian
    for dx, name in [(0.6, 'Debian'), (1.8, 'Ubuntu'), (3.0, 'Mint/\nKali')]:
        box(ax, dx, 4.0, 1.1, 0.55, name, '#e74c3c', fs=7, tc='white')
        ax.plot([1.8, dx], [5.4, 4.3], '-', color='#e74c3c', lw=0.7, alpha=0.5)

    # Children for RHEL
    for rx, name in [(3.8, 'RHEL'), (5.2, 'CentOS\nStream'), (6.5, 'Fedora\nRocky')]:
        box(ax, rx, 4.0, 1.2, 0.55, name, '#3498db', fs=7, tc='white')
        ax.plot([5.0, rx], [5.4, 4.3], '-', color='#3498db', lw=0.7, alpha=0.5)

    # Children for SUSE/Arch
    for sx, name in [(8.0, 'openSUSE'), (9.3, 'Arch'), (10.5, 'Manjaro')]:
        box(ax, sx, 4.0, 1.1, 0.55, name, '#2ecc71', fs=7, tc='white')
        ax.plot([9.0, sx], [5.4, 4.3], '-', color='#2ecc71', lw=0.7, alpha=0.5)

    # Recommendation
    ax.text(7, 2.5, '入门推荐: Ubuntu/Debian  |  企业认证: RHEL/Rocky  |  深入学习: Arch',
            ha='center', fontsize=11, fontweight='bold',
            bbox=dict(boxstyle='round', fc='#e8f8f5', ec='#2ecc71', alpha=0.9))

    save(fig, 'chart_linux_distros.png')

# ==================================================================
# 图5: 权限模型说明
# ==================================================================

def chart_permissions():
    fig, ax = plt.subplots(figsize=(13, 5))
    ax.set_xlim(0, 13); ax.set_ylim(0, 6); ax.axis('off')
    ax.set_title('Linux 权限模型 (rwx)', fontsize=15, fontweight='bold', pad=15)

    # Permission string
    ax.text(6.5, 5.0, '-  rwx  r-x  r--', fontsize=18, fontfamily='monospace',
            fontweight='bold', color='#2c3e50', ha='center',
            bbox=dict(boxstyle='round', fc='#f8f9fa', ec='#2c3e50'))

    # Labels
    y = 4.0
    box(ax, 2.2, y, 1.8, 0.6, '文件类型\n(- 文件, d 目录)', '#7f8c8d', fs=8)
    box(ax, 5.8, y, 2.2, 0.6, '所有者 (Owner)\nrwx', '#3498db', fs=8)
    box(ax, 8.8, y, 2.2, 0.6, '所属组 (Group)\nr-x', '#2ecc71', fs=8)
    box(ax, 12.0, y, 2.0, 0.6, '其他人 (Others)\nr--', '#f39c12', fs=8)

    # rwx explain
    perms = [
        (2.5, 2.2, 'r = Read (4)\n读文件内容\n列出目录'),
        (6.5, 2.2, 'w = Write (2)\n修改文件内容\n创建/删除目录文件'),
        (10.5, 2.2, 'x = eXecute (1)\n执行文件(脚本/程序)\n进入目录(cd)'),
    ]
    for x, y, label in perms:
        box(ax, x, y, 3.2, 1.2, label, '#ecf0f1', fs=8, tc='#2c3e50')

    # Octal examples
    ax.text(6.5, 0.8, 'chmod 755 = rwxr-xr-x  |  chmod 644 = rw-r--r--  |  777 = rwxrwxrwx (危险!)',
            ha='center', fontsize=10, fontweight='bold', color='#c0392b')

    save(fig, 'chart_linux_permissions.png')

# ==================================================================
# 主函数
# ==================================================================

def main():
    print('正在生成 Linux 学习图表...')
    chart_roadmap()
    chart_fhs()
    chart_boot()
    chart_distros()
    chart_permissions()
    print(f'\n全部 5 张图表已保存到 {OUT}/')

if __name__ == '__main__':
    main()
