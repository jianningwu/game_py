#!/usr/bin/env python3
"""
================================================================================
脚本功能：生成 STP / RSTP / MSTP 网络协议学习图表
- 图1: 环路问题示意（广播风暴）
- 图2: STP 选举过程（根桥/根端口/指定端口/阻塞端口）
- 图3: STP 端口状态机（5种状态转换）
- 图4: RSTP 端口角色 & P/A 握手
- 图5: MSTP 多实例拓扑 & VLAN负载均衡
- 图6: 三代协议演进对比时间线
================================================================================
"""
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
from matplotlib.patches import FancyBboxPatch, FancyArrowPatch
import numpy as np

plt.rcParams['font.family'] = ['Microsoft YaHei', 'SimHei', 'sans-serif']
plt.rcParams['axes.unicode_minus'] = False

OUT_DIR = '.'

def box(ax, x, y, w, h, text, color='#3498db', fs=9, tc='white'):
    r = FancyBboxPatch((x-w/2, y-h/2), w, h, boxstyle="round,pad=0.1",
                       fc=color, ec='#2c3e50', lw=1.5, alpha=0.92)
    ax.add_patch(r)
    ax.text(x, y, text, ha='center', va='center', fontsize=fs, color=tc, fontweight='bold')

def arrow(ax, x1, y1, x2, y2, label='', color='#555', lw=1.5):
    ax.annotate('', xy=(x2, y2), xytext=(x1, y1),
                arrowprops=dict(arrowstyle='->', color=color, lw=lw))
    if label:
        mx, my = (x1+x2)/2, (y1+y2)/2
        ax.text(mx+0.15, my+0.15, label, fontsize=8, color='#e74c3c', fontweight='bold')

def curved_arrow(ax, x1, y1, x2, y2, label='', color='#555', rad=0.3):
    ax.annotate('', xy=(x2, y2), xytext=(x1, y1),
                arrowprops=dict(arrowstyle='->', color=color, lw=1.5,
                                connectionstyle=f'arc3,rad={rad}'))
    if label:
        mx, my = (x1+x2)/2 + 0.5, (y1+y2)/2 - 0.3
        ax.text(mx, my, label, fontsize=8, color='#c0392b', fontweight='bold')

def save(fig, name):
    path = f'{OUT_DIR}/{name}'
    fig.savefig(path, dpi=150, bbox_inches='tight', facecolor='white')
    plt.close(fig)
    print(f'  [OK] {path}')

# ==================================================================
# 图1: 环路问题示意
# ==================================================================

def chart_loop_problem():
    fig, axes = plt.subplots(1, 3, figsize=(16, 5.5))
    fig.suptitle('二层环路带来的三大问题', fontsize=16, fontweight='bold', y=1.02)

    # Panel 1: 广播风暴
    ax = axes[0]
    ax.set_xlim(0, 6); ax.set_ylim(0, 6); ax.axis('off')
    ax.set_title('1. 广播风暴', fontsize=13, fontweight='bold')
    box(ax, 1.5, 5, 1.5, 0.7, 'SW-1', '#3498db')
    box(ax, 4.5, 5, 1.5, 0.7, 'SW-2', '#3498db')
    box(ax, 3, 1, 2, 0.7, '主机A 发广播帧', '#e74c3c')
    arrow(ax, 2.25, 5, 3.75, 5, '转发', '#e74c3c', 4)
    arrow(ax, 3.75, 4.8, 2.25, 4.8, '回传', '#e74c3c', 4)
    # 虚线环路
    ax.annotate('', xy=(2.0, 4.5), xytext=(4.0, 5.5),
                arrowprops=dict(arrowstyle='->', color='red', lw=2,
                                connectionstyle='arc3,rad=0.5', ls='--'))
    ax.text(3, 5.8, '无限循环!', fontsize=11, color='red', fontweight='bold', ha='center')
    arrow(ax, 3, 1.35, 2.25, 3.5, '', '#999')
    arrow(ax, 3, 1.35, 3.75, 3.5, '', '#999')

    # Panel 2: MAC表震荡
    ax = axes[1]
    ax.set_xlim(0, 6); ax.set_ylim(0, 6); ax.axis('off')
    ax.set_title('2. MAC 表震荡', fontsize=13, fontweight='bold')
    box(ax, 1.5, 5, 2.2, 0.7, 'SW-1 MAC表', '#2ecc71')
    box(ax, 4.5, 5, 2.2, 0.7, 'SW-2 MAC表', '#e67e22')
    box(ax, 3, 1, 2.5, 0.7, '主机A MAC: aa:bb:cc', '#3498db')
    # MAC 表内容
    ax.text(1.5, 4.3, 'Port1 -> aa:bb:cc\nPort2 -> aa:bb:cc\n反复切换!',
            ha='center', fontsize=8, color='red',
            bbox=dict(boxstyle='round', fc='#ffeaea', alpha=0.9))
    ax.text(4.5, 4.3, 'Port1 -> aa:bb:cc\nPort2 -> aa:bb:cc\n反复切换!',
            ha='center', fontsize=8, color='red',
            bbox=dict(boxstyle='round', fc='#ffeaea', alpha=0.9))

    # Panel 3: 多帧副本
    ax = axes[2]
    ax.set_xlim(0, 6); ax.set_ylim(0, 6); ax.axis('off')
    ax.set_title('3. 多帧副本', fontsize=13, fontweight='bold')
    box(ax, 1.5, 5, 1.5, 0.7, 'SW-1', '#3498db')
    box(ax, 4.5, 5, 1.5, 0.7, 'SW-2', '#9b59b6')
    box(ax, 1.5, 1, 1.8, 0.7, '源主机', '#3498db')
    box(ax, 4.5, 1, 1.8, 0.7, '目的主机\n收到3份!', '#e74c3c', fs=8)
    arrow(ax, 1.5, 1.35, 1.5, 3.5, '帧', '#555')
    arrow(ax, 1.5, 3.5, 4.5, 3.5, '帧', '#555')
    arrow(ax, 4.5, 3.5, 4.5, 1.35, '帧', '#555')
    curved_arrow(ax, 2.0, 4.5, 4.0, 2.5, '额外路径', '#e74c3c', 0.8)

    save(fig, 'chart01_loop_problem.png')

# ==================================================================
# 图2: STP 选举过程
# ==================================================================

def chart_stp_election():
    fig, axes = plt.subplots(1, 3, figsize=(17, 6))
    fig.suptitle('STP 选举四步流程', fontsize=15, fontweight='bold', y=1.02)

    # 拓扑
    for idx, title in enumerate(['Step 1: 选举根桥\n(最小 BID)',
                                  'Step 2-3: 选举根端口\n(最小路径成本)',
                                  'Step 4: 阻塞其余端口\n(树形拓扑完成)']):
        ax = axes[idx]
        ax.set_xlim(0, 8); ax.set_ylim(0, 7); ax.axis('off')
        ax.set_title(title, fontsize=11, fontweight='bold')

        # Switch positions
        nodes = {'A': (4, 6.2), 'B': (1.5, 3.5), 'C': (4, 3.5), 'D': (6.5, 3.5)}

        # Colors by role
        if idx == 0:
            colors = {'A': '#2ecc71', 'B': '#3498db', 'C': '#3498db', 'D': '#3498db'}
            labels = {'A': 'SW-A\nroot bridge!', 'B': 'SW-B\n(4096)', 'C': 'SW-C\n(32768)', 'D': 'SW-D\n(32768)'}
        elif idx == 1:
            colors = {'A': '#2ecc71', 'B': '#f39c12', 'C': '#f39c12', 'D': '#f39c12'}
            labels = {'A': 'SW-A(根)', 'B': 'SW-B\nRP->A', 'C': 'SW-C\nRP->A', 'D': 'SW-D\nRP->B'}
        else:
            colors = {'A': '#2ecc71', 'B': '#95a5a6', 'C': '#e74c3c', 'D': '#95a5a6'}
            labels = {'A': 'SW-A(根桥)', 'B': 'SW-B\n(冗余阻塞)', 'C': 'SW-C\n(转发)', 'D': 'SW-D\n(阻塞端口)'}

        for name, (nx, ny) in nodes.items():
            box(ax, nx, ny, 1.5, 0.8, labels[name], colors[name], fs=8)

        # Edges
        edges = [('A','B'), ('A','C'), ('B','C'), ('B','D'), ('C','D')]
        for s, e in edges:
            sx, sy = nodes[s]; ex, ey = nodes[e]
            if idx == 2 and e == 'B' and s not in ('A',):
                ax.plot([sx, ex], [sy, ey], '--', color='red', lw=2)
            elif idx == 2 and s == 'C' and e == 'D':
                ax.plot([sx, ex], [sy, ey], '--', color='red', lw=2)
            else:
                ax.plot([sx, ex], [sy, ey], '-', color='#555', lw=1.5)

        if idx == 2:
            ax.text(5.5, 2.2, 'X 阻塞', color='red', fontsize=10, fontweight='bold')

    save(fig, 'chart02_stp_election.png')

# ==================================================================
# 图3: STP 端口状态机
# ==================================================================

def chart_stp_states():
    fig, ax = plt.subplots(figsize=(12, 7))
    ax.set_xlim(0, 12); ax.set_ylim(0, 8); ax.axis('off')
    ax.set_title('STP 端口状态机（5种状态，收敛需 30~50秒）', fontsize=14, fontweight='bold', pad=15)

    states = [
        (2, 6.5, 'Disabled\n(禁用)', '#7f8c8d', '管理员 shutdown'),
        (6, 6.5, 'Blocking\n(阻塞 20s)', '#e74c3c', '只收 BPDU\n等 Max Age'),
        (6, 4.5, 'Listening\n(监听 15s)', '#f39c12', '收/发 BPDU\n不学 MAC'),
        (6, 2.5, 'Learning\n(学习 15s)', '#3498db', '学 MAC 表\n不转发'),
        (10, 4.5, 'Forwarding\n(转发)', '#2ecc71', '正常转发\n数据帧'),
    ]

    for x, y, label, color, desc in states:
        box(ax, x, y, 2.0, 1.2, label, color, fs=9)
        ax.text(x, y - 1.0, desc, ha='center', fontsize=7, color='#555')

    # Arrows between states
    arrows_stp = [
        (3.0, 6.5, 5.0, 6.5, '启用端口', '#555'),
        (6.0, 6.0, 6.0, 5.1, 'Max Age\n超时(20s)', '#e74c3c'),
        (6.0, 3.9, 6.0, 3.1, 'Forward\nDelay(15s)', '#f39c12'),
        (7.0, 4.5, 9.0, 4.5, 'Forward Delay(15s)', '#3498db'),
        (10.0, 3.9, 8.0, 2.5, '链路故障\n回退', '#e74c3c'),
    ]
    for x1,y1,x2,y2,label,color in arrows_stp:
        arrow(ax, x1, y1, x2, y2, label, color, 1.8)

    # Blocking -> Disabled
    arrow(ax, 3.0, 6.5, 5.0, 7.0, 'shutdown', '#7f8c8d')

    # Time annotation
    ax.text(8.5, 7.5, '总时间 = 20s(Max Age)\n+ 15s(Forward Delay)\n+ 15s(Forward Delay)\n= 最长 50 秒!',
            fontsize=10, color='red', fontweight='bold',
            bbox=dict(boxstyle='round', fc='#ffeaea', alpha=0.9))

    save(fig, 'chart03_stp_states.png')

# ==================================================================
# 图4: RSTP 端口角色与 P/A 握手
# ==================================================================

def chart_rstp():
    fig, axes = plt.subplots(1, 2, figsize=(16, 6.5))
    fig.suptitle('RSTP 核心机制：端口角色 + P/A 快速握手', fontsize=14, fontweight='bold', y=1.02)

    # --- Panel 1: Port Roles ---
    ax = axes[0]
    ax.set_xlim(0, 8); ax.set_ylim(0, 7); ax.axis('off')
    ax.set_title('RSTP 5种端口角色', fontsize=12, fontweight='bold')

    box(ax, 4, 6.5, 2, 0.7, '根桥', '#2ecc71')
    box(ax, 1.3, 3.5, 1.6, 0.6, 'SW-X', '#3498db')
    box(ax, 4, 3.5, 1.6, 0.6, 'SW-Y', '#3498db')
    box(ax, 6.7, 3.5, 1.6, 0.6, 'SW-Z', '#3498db')

    # Connections
    ax.plot([4, 1.3], [5.8, 3.8], '-', color='#555', lw=1.5)
    ax.plot([4, 4], [5.8, 3.8], '-', color='#555', lw=1.5)
    ax.plot([4, 6.7], [5.8, 3.8], '-', color='#555', lw=1.5)
    ax.plot([1.3, 4], [3.2, 3.2], '-', color='#555', lw=1.5)
    ax.plot([4, 6.7], [3.2, 3.2], '-', color='#555', lw=1.5)

    ax.text(2.6, 5.0, 'Root Port', fontsize=8, color='#2ecc71', fontweight='bold')
    ax.text(4.5, 5.0, 'Root Port', fontsize=8, color='#2ecc71', fontweight='bold')
    ax.text(5.5, 5.0, 'Alternate\n(备份RP)', fontsize=7, color='#f39c12', fontweight='bold')

    # Edge port
    box(ax, 1.3, 1, 2.2, 0.6, 'Edge Port -> PC', '#1abc9c', fs=8)
    ax.plot([1.3, 1.3], [3.2, 1.3], '-', color='#1abc9c', lw=2)
    ax.text(0.5, 2.2, 'PortFast\n直接转发!', fontsize=7, color='#1abc9c', fontweight='bold')

    # --- Panel 2: P/A Handshake ---
    ax = axes[1]
    ax.set_xlim(0, 10); ax.set_ylim(0, 8); ax.axis('off')
    ax.set_title('P/A 握手流程（秒级收敛）', fontsize=12, fontweight='bold')

    box(ax, 3, 7, 2.5, 0.7, '根桥 (SW-A)', '#2ecc71', fs=10)
    box(ax, 3, 2, 2.5, 0.7, '非根桥 (SW-B)', '#3498db', fs=10)

    steps = [
        (3, 6.3, 3, 3, '① Proposal\n  我想成为指定端口', '#3498db'),
        (5.5, 6.3, 5.5, 3.7, '② Sync: SW-B 阻塞\n   其他端口', '#f39c12'),
        (7.5, 3, 7.5, 5.3, '③ Agreement\n   我已准备好!', '#2ecc71'),
        (1, 3, 1, 5.3, '④ 端口立即 Forwarding\n  (不等计时器!)', '#e74c3c'),
    ]

    for x1, y1, x2, y2, label, color in steps:
        arrow(ax, x1, y1, x2, y2, label, color, 2)

    ax.text(5, 4.5, 'P/A 握手\n< 1 秒完成!', fontsize=10, color='red', fontweight='bold',
            bbox=dict(boxstyle='round', fc='#ffffcc', alpha=0.9), ha='center')

    save(fig, 'chart04_rstp.png')

# ==================================================================
# 图5: MSTP 多实例与负载均衡
# ==================================================================

def chart_mstp():
    fig, axes = plt.subplots(1, 2, figsize=(16, 6.5))
    fig.suptitle('MSTP 多实例拓扑 — 不同 VLAN 走不同路径', fontsize=14, fontweight='bold', y=1.02)

    sw_pos = {'A': (4, 1.5), 'B': (1, 5), 'C': (7, 5)}
    sw_color = '#3498db'

    for idx, (title, root, block_edge, active_vlans, block_vlans) in enumerate([
        ('实例 1 (MSTI 1)\nVLAN 10,20 走路径 A-C',
         'A', [('A','B'), ('A','B')], 'VLAN 10,20', 'VLAN 30,40'),
        ('实例 2 (MSTI 2)\nVLAN 30,40 走路径 A-B',
         'A', [('A','C')], 'VLAN 30,40', 'VLAN 10,20'),
    ]):
        ax = axes[idx]
        ax.set_xlim(0, 10); ax.set_ylim(0, 7); ax.axis('off')
        ax.set_title(title, fontsize=11, fontweight='bold')

        for name, (nx, ny) in sw_pos.items():
            c = '#2ecc71' if name == root else sw_color
            box(ax, nx, ny, 1.6, 0.7, f'SW-{name}', c, fs=10)

        # All edges
        for s, e in [('A','B'), ('A','C'), ('B','C')]:
            sx, sy = sw_pos[s]; ex, ey = sw_pos[e]
            ax.plot([sx, ex], [sy, ey], '-', color='#555', lw=1.5)

        # Blocked link
        if idx == 0:
            # Block A-B for MSTI 1
            ax.plot([4, 1.0], [2.2, 4.6], '--', color='red', lw=2)
            ax.text(2.0, 3.2, 'X 阻塞', color='red', fontsize=9, fontweight='bold')
            # Active path highlight
            ax.text(5.5, 4.0, f'{active_vlans}\n(活跃)', fontsize=8, color='#2ecc71',
                    fontweight='bold', ha='center')
        else:
            ax.plot([4, 7.0], [2.2, 4.6], '--', color='red', lw=2)
            ax.text(6.0, 3.2, 'X 阻塞', color='red', fontsize=9, fontweight='bold')
            ax.text(2.0, 4.5, f'{active_vlans}\n(活跃)', fontsize=8, color='#2ecc71',
                    fontweight='bold', ha='center')

        # VLAN tag
        ax.text(4, 0.5, f'同一物理链路，{active_vlans}走此路径',
                ha='center', fontsize=9, fontweight='bold',
                bbox=dict(boxstyle='round', fc='#e8f8f5', alpha=0.9))

    # Load balancing explanation
    fig.text(0.5, 0.01, '负载均衡: VLAN 10,20 -> SW-C 转发 | VLAN 30,40 -> SW-B 转发',
             ha='center', fontsize=11, fontweight='bold', color='#e74c3c')

    save(fig, 'chart05_mstp_instances.png')

# ==================================================================
# 图6: 三代协议对比时间线
# ==================================================================

def chart_evolution():
    fig, ax = plt.subplots(figsize=(14, 5))
    ax.set_xlim(1985, 2025); ax.set_ylim(-2, 4); ax.axis('off')
    ax.set_title('生成树协议演进时间线', fontsize=15, fontweight='bold', pad=15)

    # Timeline
    ax.axhline(y=0, color='#ccc', lw=3, zorder=1)

    events = [
        (1990, 1.5, 'STP\n802.1D', '#e74c3c',
         '解决环路\n30-50s收敛\n5种状态'),
        (2001, 2.5, 'RSTP\n802.1w', '#f39c12',
         '快速收敛(<1s)\nP/A握手\n3种状态'),
        (2002, 1.2, 'MSTP\n802.1s', '#2ecc71',
         '多实例\nVLAN负载均衡\n域间互通'),
        (2012, 0.8, 'SPB\n802.1aq', '#9b59b6',
         '最短路径桥接\nIS-IS替代STP\n数据中心首选'),
    ]

    for year, y, title, color, desc in events:
        ax.plot(year, 0, 'o', color=color, markersize=14, zorder=5)
        ax.vlines(year, 0, y, color=color, lw=2, alpha=0.5)
        box(ax, year, y + 0.5, 1.6, 0.7, title, color, fs=8)
        ax.text(year, y - 0.6, desc, ha='center', fontsize=7, color='#555',
                bbox=dict(boxstyle='round', fc='#f8f9fa', alpha=0.85))

    # Key feature arrows
    ax.annotate('收敛加速', xy=(2001, 3.2), xytext=(1990, 3.5),
                arrowprops=dict(arrowstyle='->', color='#e74c3c', lw=2), fontsize=10)
    ax.annotate('资源优化', xy=(2002, 3.2), xytext=(2001, 3.5),
                arrowprops=dict(arrowstyle='->', color='#f39c12', lw=2), fontsize=10)

    save(fig, 'chart06_evolution_timeline.png')

# ==================================================================
# 主函数
# ==================================================================

def main():
    print('正在生成 STP/RSTP/MSTP 图表...')
    chart_loop_problem()
    chart_stp_election()
    chart_stp_states()
    chart_rstp()
    chart_mstp()
    chart_evolution()
    print(f'\n全部 6 张图表已保存到 {OUT_DIR}/')

if __name__ == '__main__':
    main()
