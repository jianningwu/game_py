"""
飞机大战 - Aircraft War Game
使用键盘控制飞机移动、射击，击败敌机获取分数。
"""
import pygame
import random
import sys
import math

# 初始化 Pygame 库，启动所有 Pygame 模块
pygame.init()

# ---------- 游戏常量 ----------
# 屏幕宽高（像素）
SCREEN_WIDTH = 480
SCREEN_HEIGHT = 700
# 游戏帧率（每秒更新 60 帧）
FPS = 60

# ---------- 颜色 RGB 常量 ----------
BLACK = (0, 0, 0)           # 纯黑
WHITE = (255, 255, 255)     # 纯白
GREEN = (0, 255, 0)         # 绿色，玩家飞机主色
RED = (255, 0, 0)           # 红色，敌机/生命值
BLUE = (0, 100, 255)        # 蓝色，火力升级道具
YELLOW = (255, 255, 0)      # 黄色，普通子弹
ORANGE = (255, 165, 0)      # 橙色，散射子弹
GRAY = (100, 100, 100)      # 灰色，菜单提示
SKY_BLUE = (30, 30, 80)     # 深空蓝，背景色

# ---------- 游戏状态枚举 ----------
MENU = 0      # 菜单界面
PLAYING = 1   # 游戏进行中
GAME_OVER = 2 # 游戏结束


# ---------------------------------------------------------------------------
# 工具函数：在画面上绘制文字
# ---------------------------------------------------------------------------
def draw_text(surface, text, size, x, y, color=WHITE, center=True):
    """
    在指定 Surface 上绘制文字。

    参数:
        surface: 要绘制到的 Pygame Surface 对象
        text: 要显示的文字内容
        size: 字号大小
        x, y: 文字位置坐标
        color: 文字颜色（RGB 元组）
        center: True 表示 (x,y) 是文字中心，False 表示 (x,y) 是左上角
    """
    # 使用黑体字体（simhei）渲染文字
    font = pygame.font.SysFont("simhei", size)
    # 将文字渲染为图片（抗锯齿）
    img = font.render(text, True, color)
    rect = img.get_rect()
    if center:
        # 居中对齐：rect 中心对准 (x, y)
        rect.center = (x, y)
    else:
        # 左对齐：rect 左上角在 (x, y)
        rect.topleft = (x, y)
    # 将文字图片绘制到目标 surface 上
    surface.blit(img, rect)


# ---------------------------------------------------------------------------
# 玩家飞机类
# 继承自 pygame.sprite.Sprite，可以使用精灵组进行碰撞检测和批量更新
# ---------------------------------------------------------------------------
class Player(pygame.sprite.Sprite):
    """玩家飞机：可移动、射击、受伤无敌、火力升级"""

    def __init__(self):
        """初始化玩家飞机：位置、速度、生命值、火力等级"""
        super().__init__()
        # 飞机尺寸（宽度 x 高度，像素）
        self.width = 40
        self.height = 50
        # 用 _create_surface() 绘制飞机外观，生成 Surface 图像
        self.image = self._create_surface()
        # 获取图像的矩形区域，用于定位和碰撞检测
        self.rect = self.image.get_rect()
        # 初始位置：屏幕底部中央
        self.rect.centerx = SCREEN_WIDTH // 2
        self.rect.bottom = SCREEN_HEIGHT - 30
        # 移动速度（像素/帧）
        self.speed = 6
        # 生命值（被击中后减 1，归零则游戏结束）
        self.lives = 3
        # 无敌帧计数器（受伤后一段时间内不再次受伤）
        self.invincible = 0
        # 射击冷却时间（毫秒），控制射速
        self.shoot_delay = 150
        # 上一次射击的时刻（毫秒），用于计算冷却
        self.last_shot = 0
        # 火力等级：1=单发，2=双发，3=三发散弹
        self.power = 1

    def _create_surface(self):
        """
        绘制玩家飞机的外观。
        使用多边形绘制一个箭头形的绿色飞机，
        带蓝色座舱盖。
        返回一个带有透明背景的 Surface。
        """
        # SRCALPHA 表示支持透明通道
        surf = pygame.Surface((self.width, self.height), pygame.SRCALPHA)
        # ---------- 飞机主体（绿色多边形） ----------
        # 顶点顺序说明：
        # 顶点0: 机头（顶部中央）
        # 顶点1: 右翼上沿
        # 顶点2: 右翼中部收缩
        # 顶点3: 右尾翼
        # 顶点4: 机身右侧底部
        # 顶点5: 机身底部中央
        # 顶点6: 机身左侧底部
        # 顶点7: 左尾翼
        # 顶点8: 左翼中部收缩
        # 顶点9: 左翼上沿
        pygame.draw.polygon(surf, GREEN, [
            (self.width // 2, 0),               # 顶点0：机头
            (self.width, self.height // 3),       # 顶点1：右翼尖
            (self.width - 5, self.height // 2),   # 顶点2：右翼内侧
            (self.width, self.height - 5),        # 顶点3：右尾翼
            (self.width // 2 + 3, self.height - 10),  # 顶点4：机身右下
            (self.width // 2, self.height),       # 顶点5：机尾中央
            (self.width // 2 - 3, self.height - 10),  # 顶点6：机身左下
            (0, self.height - 5),                 # 顶点7：左尾翼
            (5, self.height // 2),                # 顶点8：左翼内侧
            (0, self.height // 3),                # 顶点9：左翼尖
        ])
        # ---------- 座舱盖（蓝色圆形） ----------
        pygame.draw.circle(surf, BLUE, (self.width // 2, self.height // 3), 5)
        return surf

    def update(self):
        """每帧更新：处理无敌闪烁效果"""
        if self.invincible > 0:
            # 无敌倒计时递减
            self.invincible -= 1
            # 每 5 帧切换一次透明度，产生闪烁效果
            if (self.invincible // 5) % 2:
                self.image.set_alpha(128)  # 半透明
            else:
                self.image.set_alpha(255)  # 不透明
        else:
            # 非无敌状态，保持完全不透明
            self.image.set_alpha(255)

    def move(self, dx, dy):
        """
        移动飞机，并限制在屏幕范围内。

        参数:
            dx: x 方向偏移（正数向右，负数向左）
            dy: y 方向偏移（正数向下，负数向上）
        """
        self.rect.x += dx
        self.rect.y += dy
        # clamp_ip 将矩形限制在指定区域内，不超出屏幕边界
        self.rect.clamp_ip(pygame.Rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT))

    def shoot(self, bullets):
        """
        发射子弹。

        根据当前火力等级（power）发射不同数量和方向的子弹：
        - Lv.1: 单发，从机头射出
        - Lv.2: 双发，从机头两侧射出
        - Lv.3: 三发散弹，左右两侧 + 机头一枚稍快

        参数:
            bullets: 子弹精灵组，新生成的子弹会加入此组
        """
        # 获取当前时间（毫秒），检查是否已过冷却期
        now = pygame.time.get_ticks()
        if now - self.last_shot < self.shoot_delay:
            return  # 冷却未结束，不能射击
        self.last_shot = now  # 记录本次射击时间

        # 计算机头位置作为子弹发射原点
        cx, cy = self.rect.centerx, self.rect.top
        if self.power == 1:
            # 火力1：单发居中
            bullets.add(Bullet(cx, cy, 0))
        elif self.power == 2:
            # 火力2：双发，左右各偏 8 像素
            bullets.add(Bullet(cx - 8, cy, 0))
            bullets.add(Bullet(cx + 8, cy, 0))
        else:  # power == 3
            # 火力3：三发，左中右
            bullets.add(Bullet(cx - 14, cy, 0))  # 左
            bullets.add(Bullet(cx, cy - 5, 0))    # 中（稍微靠前发射）
            bullets.add(Bullet(cx + 14, cy, 0))   # 右

    def hit(self):
        """
        玩家被击中处理：
        - 如果处于无敌状态，不扣血
        - 否则生命值减 1，进入 1.5 秒无敌状态
        - 如果生命归零则返回 True（游戏结束）

        返回:
            True 表示玩家已无生命，游戏结束
            False 表示还有剩余生命
        """
        if self.invincible > 0:
            return False  # 无敌状态，忽略本次伤害
        self.lives -= 1                 # 生命值减 1
        self.invincible = 90            # 设置无敌时间：90帧 = 1.5秒（60fps）
        return self.lives <= 0          # 生命归零则游戏结束

    def power_up(self):
        """提升火力等级，最高 3 级"""
        if self.power < 3:
            self.power += 1


# ---------------------------------------------------------------------------
# 玩家子弹类
# 从玩家飞机向上飞行的黄色/橙色子弹
# ---------------------------------------------------------------------------
class Bullet(pygame.sprite.Sprite):
    """玩家发射的子弹，向上飞行，超出屏幕自动销毁"""

    def __init__(self, x, y, speed_x=0):
        """
        初始化子弹。

        参数:
            x: 子弹出生 x 坐标（中心）
            y: 子弹出生 y 坐标（底部）
            speed_x: 水平偏移速度（用于散弹效果，默认 0 表示直射）
        """
        super().__init__()
        self.speed_x = speed_x
        # 子弹尺寸：6x14 像素
        self.image = pygame.Surface((6, 14), pygame.SRCALPHA)
        if speed_x == 0:
            self.image.fill(YELLOW)  # 直射子弹为黄色
        else:
            self.image.fill(ORANGE)  # 带偏角的子弹为橙色
        self.rect = self.image.get_rect()
        self.rect.centerx = x
        self.rect.bottom = y

    def update(self):
        """每帧向上移动，超出屏幕顶部则销毁"""
        self.rect.y -= 8          # 向上飞行（每帧 8 像素）
        self.rect.x += self.speed_x  # 水平偏移（散弹效果）
        if self.rect.bottom < 0:  # 完全移出屏幕顶部
            self.kill()           # 从所有精灵组中移除


# ---------------------------------------------------------------------------
# 敌机子弹类
# 从敌机向玩家射出的红色圆形子弹，可带角度
# ---------------------------------------------------------------------------
class EnemyBullet(pygame.sprite.Sprite):
    """敌机发射的子弹，向下飞行，超出屏幕任意方向则销毁"""

    def __init__(self, x, y, vx=0, vy=4):
        """
        初始化敌机子弹。

        参数:
            x, y: 子弹起始位置
            vx: 水平速度分量
            vy: 垂直速度分量（默认 4，向下）
        """
        super().__init__()
        # 红色小圆形子弹，6x6 像素
        self.image = pygame.Surface((6, 6), pygame.SRCALPHA)
        pygame.draw.circle(self.image, RED, (3, 3), 3)
        self.rect = self.image.get_rect()
        self.rect.centerx = x
        self.rect.top = y
        self.vx = vx  # 水平速度
        self.vy = vy  # 垂直速度

    def update(self):
        """每帧按速度移动，超出屏幕边界则销毁"""
        self.rect.x += self.vx
        self.rect.y += self.vy
        # 如果子弹超出屏幕任意边界（上下左右），则销毁
        if self.rect.top > SCREEN_HEIGHT or self.rect.bottom < 0 or \
           self.rect.right < 0 or self.rect.left > SCREEN_WIDTH:
            self.kill()


# ---------------------------------------------------------------------------
# 敌机基类
# 定义了所有敌机的通用属性和行为
# ---------------------------------------------------------------------------
class Enemy(pygame.sprite.Sprite):
    """敌机基类，包含生命值、速度、射击、受伤等通用行为"""

    def __init__(self, hp=1, speed=2, size=(40, 30)):
        """
        初始化敌机。

        参数:
            hp: 生命值（需被击中多少次才摧毁）
            speed: 下落速度（像素/帧）
            size: (宽度, 高度) 元组
        """
        super().__init__()
        self.hp = hp                  # 当前生命值
        self.max_hp = hp              # 最大生命值（备用，未用到）
        self.base_speed = speed       # 基础速度（备用）
        self.speed = speed            # 当前下落速度
        self.width, self.height = size  # 敌机尺寸
        self.image = self._create_surface()  # 绘制外观
        self.rect = self.image.get_rect()
        # 从屏幕顶部外随机水平位置出现
        self.rect.x = random.randint(0, SCREEN_WIDTH - self.width)
        self.rect.y = -self.height     # 从屏幕顶部上方进入
        self.last_shot = 0             # 上次射击时间
        self.shoot_delay = random.randint(1000, 3000)  # 射击间隔（毫秒）

    def _create_surface(self):
        """
        创建敌机外观的 Surface。
        子类应重写此方法绘制不同的敌机样式。
        此处返回一个透明 Surface。
        """
        surf = pygame.Surface((self.width, self.height), pygame.SRCALPHA)
        return surf

    def update(self):
        """每帧向下移动，超出屏幕底部则销毁"""
        self.rect.y += self.speed
        if self.rect.top > SCREEN_HEIGHT:
            self.kill()  # 飞出屏幕底部的敌机从游戏中移除

    def shoot(self, bullets):
        """
        敌机开火：向下发射一颗子弹。

        带冷却时间，不到冷却间隔不发射。

        参数:
            bullets: 敌机子弹精灵组
        """
        now = pygame.time.get_ticks()
        if now - self.last_shot < self.shoot_delay:
            return  # 冷却中
        self.last_shot = now
        bullets.add(EnemyBullet(self.rect.centerx, self.rect.bottom))

    def hit(self, damage=1):
        """
        敌机被击中处理：
        - 生命值减去伤害值
        - 闪白表示受伤
        - 如果生命归零返回 True（摧毁）

        参数:
            damage: 伤害值，默认 1

        返回:
            True 表示敌机被摧毁，False 表示仍存活
        """
        self.hp -= damage
        # 受伤闪白（填充白色）
        self.image.fill(WHITE)
        return self.hp <= 0


# ---------------------------------------------------------------------------
# 小型敌机类
# 低生命、高速度、低分数，不会射击
# ---------------------------------------------------------------------------
class SmallEnemy(Enemy):
    """小型敌机：1 点生命，速度快，不射击，100 分"""

    def __init__(self):
        super().__init__(hp=1, speed=3, size=(30, 24))
        self.score = 100  # 击落后获得的分数

    def _create_surface(self):
        """绘制灰色椭圆形的小型敌机"""
        surf = super()._create_surface()
        # 灰色椭圆机身
        pygame.draw.ellipse(surf, GRAY, (2, 2, self.width - 4, self.height - 4))
        # 浅灰色驾驶舱
        pygame.draw.ellipse(surf, (150, 150, 150), (6, 6, self.width - 12, self.height // 2))
        return surf


# ---------------------------------------------------------------------------
# 普通敌机类
# 中等生命和速度，会射击，中等分数
# ---------------------------------------------------------------------------
class NormalEnemy(Enemy):
    """普通敌机：2 点生命，中速，会射击，200 分"""

    def __init__(self):
        super().__init__(hp=2, speed=2, size=(45, 35))
        self.score = 200  # 击落后获得的分数

    def _create_surface(self):
        """绘制红色三角形的普通敌机"""
        surf = super()._create_surface()
        # 红色倒三角机身
        pygame.draw.polygon(surf, RED, [
            (self.width // 2, 0),                   # 顶部顶点
            (self.width, self.height // 3),          # 右上
            (self.width - 4, self.height - 4),       # 右下
            (4, self.height - 4),                    # 左下
            (0, self.height // 3),                   # 左上
        ])
        # 深红色圆形座舱
        pygame.draw.circle(surf, (200, 0, 0), (self.width // 2, self.height // 3), 6)
        return surf


# ---------------------------------------------------------------------------
# Boss 敌机类
# 高生命、低速，进入屏幕后左右移动，快速射击，高分
# ---------------------------------------------------------------------------
class BossEnemy(Enemy):
    """Boss 敌机：20 点生命，低速，左右徘徊，快速射击，1000 分"""

    def __init__(self):
        super().__init__(hp=20, speed=1, size=(80, 60))
        self.score = 1000       # 击落后获得的分数
        self.enter_y = 80       # Boss 停住时的 Y 坐标
        self.entering = True    # True 表示正在入场（向下飞），False 表示已就位
        self.shoot_delay = 500  # Boss 射速更快（500ms 间隔）
        self.dir = 1            # 左右移动方向：1 右，-1 左

    def _create_surface(self):
        """绘制紫色多边形 Boss 敌机"""
        surf = super()._create_surface()
        # 紫色五边形机身
        pygame.draw.polygon(surf, (180, 0, 180), [
            (self.width // 2, 0),                  # 顶部
            (self.width, 15),                       # 右上
            (self.width - 5, self.height - 10),     # 右下
            (self.width // 2 + 5, self.height),     # 底部偏右
            (self.width // 2 - 5, self.height),     # 底部偏左
            (5, self.height - 10),                  # 左下
            (0, 15),                                # 左上
        ])
        # 红色大圆形座舱
        pygame.draw.circle(surf, (255, 0, 0), (self.width // 2, self.height // 3), 8)
        return surf

    def update(self):
        """
        Boss 移动逻辑：
        - 入场阶段：向下飞到指定高度
        - 就位后：左右来回移动
        - 超出屏幕底部则销毁
        """
        if self.entering:
            # 入场阶段：向下飞行
            self.rect.y += self.speed
            if self.rect.y >= self.enter_y:
                # 到达目标高度，停止入场，进入战斗状态
                self.rect.y = self.enter_y
                self.entering = False
        else:
            # 战斗状态：左右水平移动
            self.rect.x += self.dir * 2
            # 碰到屏幕左右边界则掉头
            if self.rect.right > SCREEN_WIDTH or self.rect.left < 0:
                self.dir *= -1
        # 如果 Boss 被击飞到屏幕外，销毁
        if self.rect.top > SCREEN_HEIGHT:
            self.kill()

    def hit(self, damage=1):
        """Boss 受伤：闪紫色，生命归零则摧毁"""
        self.hp -= damage
        self.image.fill((255, 0, 255))  # 闪紫色
        return self.hp <= 0


# ---------------------------------------------------------------------------
# 爆炸特效类
# 多帧动画：从黄色→橙色→红色→深红，逐渐缩小
# ---------------------------------------------------------------------------
class Explosion(pygame.sprite.Sprite):
    """爆炸动画：6 帧循环，大爆炸半径 30，小爆炸半径 15"""

    def __init__(self, x, y, big=True):
        """
        初始化爆炸特效。

        参数:
            x, y: 爆炸中心位置
            big: True 为大爆炸（Boss 被摧毁），False 为小爆炸（普通敌机）
        """
        super().__init__()
        self.images = []    # 存储各帧图像
        radius = 30 if big else 15  # 爆炸最大半径

        # 生成 6 帧爆炸动画，颜色从亮到暗
        for i in range(6):
            surf = pygame.Surface((radius * 2, radius * 2), pygame.SRCALPHA)
            # 颜色渐变：黄 → 橙 → 红 → 深红
            colors = [YELLOW, ORANGE, RED, (100, 0, 0)]
            color = colors[min(i, len(colors) - 1)]
            # 半径逐渐缩小（i 越大 r 越小）
            r = int(radius * (1 - i / 8))
            pygame.draw.circle(surf, color, (radius, radius), max(r, 2))
            self.images.append(surf)

        # 从第 0 帧开始
        self.image = self.images[0]
        self.rect = self.image.get_rect(center=(x, y))
        self.frame = 0               # 当前帧计数器
        self.max_frame = len(self.images) * 3  # 每帧持续 3 个游戏帧

    def update(self):
        """每帧更新动画帧，播完最后一帧则销毁"""
        self.frame += 1
        idx = self.frame // 3       # 每 3 帧切换一次画面
        if idx >= len(self.images):
            self.kill()             # 动画播放完毕，从游戏中移除
            return
        self.image = self.images[idx]


# ---------------------------------------------------------------------------
# 星空背景类
# 模拟星空效果，星星缓缓向下移动，形成飞行感
# ---------------------------------------------------------------------------
class Star:
    """单颗星星：随机位置、亮度、大小，持续向下移动"""

    def __init__(self):
        """初始化星星：随机位置、速度、大小和亮度"""
        self.x = random.randint(0, SCREEN_WIDTH)
        self.y = random.randint(0, SCREEN_HEIGHT)
        self.speed = random.uniform(0.5, 2)  # 下落速度
        self.r = random.randint(1, 2)         # 星星半径（1 或 2 像素）
        self.brightness = random.randint(100, 255)  # 亮度（100~255）

    def update(self):
        """每帧向下移动，超出屏幕底部则从顶部重新出现"""
        self.y += self.speed
        if self.y > SCREEN_HEIGHT:
            self.y = 0
            self.x = random.randint(0, SCREEN_WIDTH)

    def draw(self, surf):
        """在指定 Surface 上绘制星星"""
        # 灰白色圆形，亮度决定颜色值
        pygame.draw.circle(surf, (self.brightness,) * 3, (int(self.x), int(self.y)), self.r)


# ---------------------------------------------------------------------------
# 火力升级道具类
# 蓝色圆形 + "P" 字母，从敌机掉落，拾取后提升火力等级
# ---------------------------------------------------------------------------
class PowerUp(pygame.sprite.Sprite):
    """火力升级道具：蓝色圆环带 P 字母，向下飘落"""

    def __init__(self, x, y):
        """
        初始化道具。

        参数:
            x, y: 道具出现位置（击败敌机的位置）
        """
        super().__init__()
        # 绘制道具外观：蓝色圆环 + 白色内圈 + P 字母
        self.image = pygame.Surface((20, 20), pygame.SRCALPHA)
        pygame.draw.circle(self.image, BLUE, (10, 10), 10)    # 蓝色外圈
        pygame.draw.circle(self.image, WHITE, (10, 10), 6)    # 白色内圈
        draw_text(self.image, "P", 14, 10, 10, BLUE)           # 中央写 P
        self.rect = self.image.get_rect(center=(x, y))
        self.speed = 2  # 下落速度

    def update(self):
        """每帧向下移动，超出屏幕底部则销毁"""
        self.rect.y += self.speed
        if self.rect.top > SCREEN_HEIGHT:
            self.kill()


# ---------------------------------------------------------------------------
# 游戏主类
# 管理游戏循环、事件处理、碰撞检测、画面绘制
# ---------------------------------------------------------------------------
class Game:
    """游戏主控制器，管理所有游戏逻辑和渲染"""

    def __init__(self):
        """初始化游戏窗口、精灵组、统计数据和音效"""
        # ---------- 创建游戏窗口 ----------
        self.screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT))
        pygame.display.set_caption("飞机大战 - Aircraft War")
        # 帧率控制器
        self.clock = pygame.time.Clock()
        self.running = True    # 游戏主循环标志
        self.state = MENU      # 当前游戏状态（菜单/游戏中/结束）

        # ---------- 创建精灵组 ----------
        # 所有精灵组（用于一次性绘制所有对象）
        self.all_sprites = pygame.sprite.Group()
        # 玩家精灵组（GroupSingle 只能容纳一个精灵）
        self.player_group = pygame.sprite.GroupSingle()
        # 玩家子弹精灵组
        self.bullets = pygame.sprite.Group()
        # 敌机精灵组
        self.enemies = pygame.sprite.Group()
        # 敌机子弹精灵组
        self.enemy_bullets = pygame.sprite.Group()
        # 爆炸特效精灵组
        self.explosions = pygame.sprite.Group()
        # 火力道具精灵组
        self.powerups = pygame.sprite.Group()

        # 创建玩家飞机
        self.player = Player()
        # 创建 80 颗背景星星
        self.stars = [Star() for _ in range(80)]

        # ---------- 游戏统计数据 ----------
        self.score = 0               # 当前得分
        self.level = 1               # 当前等级（影响生成速度）
        self.enemy_timer = 0         # 敌机生成计时器
        self.enemy_spawn_rate = 60   # 敌机生成间隔（帧数）
        self.boss_wave = 5           # Boss 出现波次（未使用）
        self.enemies_killed = 0      # 已击败敌机数量
        self.combo = 0               # 连击数（连续击败不中断）

        # 初始化音效
        self._init_sounds()

    def _init_sounds(self):
        """
        初始化音效。
        尝试加载音效文件，如果失败则静默跳过。
        （当前实现为空音效占位，可替换为实际音频文件）
        """
        try:
            # 用空音频缓冲创建 Sound 对象占位
            self.shoot_sound = pygame.mixer.Sound(
                pygame.mixer.Sound(buffer=bytes([0] * 100))
            )
            self.explosion_sound = pygame.mixer.Sound(
                pygame.mixer.Sound(buffer=bytes([0] * 100))
            )
        except Exception:
            # 如果音频初始化失败（如无音频设备），直接忽略
            pass

    def reset(self):
        """
        重置游戏状态。
        清空所有精灵组，重新创建玩家，清零分数和等级。
        在玩家选择重新开始时调用。
        """
        # 清空所有精灵组
        self.all_sprites.empty()
        self.player_group.empty()
        self.bullets.empty()
        self.enemies.empty()
        self.enemy_bullets.empty()
        self.explosions.empty()
        self.powerups.empty()

        # 创建新玩家
        self.player = Player()
        self.player_group.add(self.player)

        # 重置统计数据
        self.score = 0
        self.level = 1
        self.enemy_timer = 0
        self.enemy_spawn_rate = 60
        self.enemies_killed = 0
        self.combo = 0

    def spawn_enemy(self):
        """
        生成一个敌机。
        - 每击败 50 个敌机生成一个 Boss
        - 否则 60% 概率小型敌机，40% 概率普通敌机
        - 根据当前等级提升敌机速度
        """
        r = random.random()
        # 每击败 50 个敌机出现一次 Boss
        if self.enemies_killed > 0 and self.enemies_killed % 50 == 0:
            enemy = BossEnemy()
        elif r < 0.6:
            enemy = SmallEnemy()
        else:
            enemy = NormalEnemy()

        # 等级越高，敌机速度越快（最多增加 2）
        enemy.speed += min(self.level * 0.2, 2)
        self.enemies.add(enemy)
        self.all_sprites.add(enemy)

    def handle_events(self):
        """
        处理用户输入事件（键盘操作、窗口关闭等）。
        不同游戏状态下按键行为不同：
        - 菜单：空格/回车开始游戏
        - 游戏中：方向键移动，空格射击，ESC 返回菜单
        - 结束：空格/回车返回菜单
        """
        for event in pygame.event.get():
            # 关闭窗口按钮
            if event.type == pygame.QUIT:
                self.running = False
                return

            # 按键事件
            if event.type == pygame.KEYDOWN:
                if self.state == MENU:
                    # 菜单界面按 空格/回车 开始游戏
                    if event.key == pygame.K_RETURN or event.key == pygame.K_SPACE:
                        self.state = PLAYING
                        self.reset()  # 重置所有游戏状态

                elif self.state == GAME_OVER:
                    # 游戏结束界面按 空格/回车 返回菜单
                    if event.key == pygame.K_RETURN or event.key == pygame.K_SPACE:
                        self.state = MENU

                elif self.state == PLAYING:
                    # 游戏中按 ESC 返回菜单
                    if event.key == pygame.K_ESCAPE:
                        self.state = MENU

    def update(self):
        """
        游戏主更新逻辑（每帧调用一次）。
        更新所有对象状态、检测碰撞、处理得分和等级。
        只在 PLAYING 状态下执行。
        """
        if self.state != PLAYING:
            return

        # ----- 1. 更新星空背景 -----
        for star in self.stars:
            star.update()

        # ----- 2. 处理玩家输入（每帧持续检测按键） -----
        keys = pygame.key.get_pressed()
        dx = dy = 0
        # 左右移动
        if keys[pygame.K_LEFT] or keys[pygame.K_a]:
            dx = -self.player.speed
        if keys[pygame.K_RIGHT] or keys[pygame.K_d]:
            dx = self.player.speed
        # 上下移动
        if keys[pygame.K_UP] or keys[pygame.K_w]:
            dy = -self.player.speed
        if keys[pygame.K_DOWN] or keys[pygame.K_s]:
            dy = self.player.speed
        # 执行移动（含边界限制）
        self.player.move(dx, dy)

        # 按住空格键持续射击（自动连发）
        if keys[pygame.K_SPACE]:
            self.player.shoot(self.bullets)

        # ----- 3. 更新所有精灵状态 -----
        self.player_group.update()   # 玩家（无敌闪烁）
        self.bullets.update()        # 玩家子弹（向上飞行）
        self.enemies.update()        # 敌机（向下移动）
        self.enemy_bullets.update()  # 敌机子弹（向下飞行）
        self.explosions.update()     # 爆炸特效（播放动画帧）
        self.powerups.update()       # 道具（向下飘落）

        # ----- 4. 敌机射击（普通敌机和 Boss 会开火） -----
        for enemy in self.enemies:
            if isinstance(enemy, (NormalEnemy, BossEnemy)):
                enemy.shoot(self.enemy_bullets)

        # ----- 5. 自动生成敌机 -----
        self.enemy_timer += 1
        # 等级越高，生成间隔越短（最快 15 帧生成一个）
        spawn_rate = max(15, self.enemy_spawn_rate - self.level * 2)
        if self.enemy_timer >= spawn_rate:
            self.enemy_timer = 0
            self.spawn_enemy()

        # ----- 6. 碰撞检测：玩家子弹 VS 敌机 -----
        # 子弹击中敌机后子弹消失（True），敌机暂不消失（False）
        hits = pygame.sprite.groupcollide(self.bullets, self.enemies, True, False)
        for bullet, enemy_list in hits.items():
            for enemy in enemy_list:
                if enemy.hit():
                    # 敌机被摧毁：加分（连击加成）、记录击败数、连击+1
                    self.score += enemy.score * (1 + self.combo // 10)
                    self.enemies_killed += 1
                    self.combo += 1
                    # Boss 用大爆炸，普通敌机用小爆炸
                    self.explosions.add(Explosion(
                        enemy.rect.centerx, enemy.rect.centery,
                        isinstance(enemy, BossEnemy)
                    ))
                    # 10% 概率掉落火力升级道具（火力满级不掉落）
                    if random.random() < 0.1 and self.player.power < 3:
                        self.powerups.add(PowerUp(enemy.rect.centerx, enemy.rect.centery))
                    enemy.kill()  # 从游戏中移除敌机
                else:
                    # 敌机未死（伤害闪烁在 Enemy.hit() 里处理了）
                    pass

        # ----- 7. 碰撞检测：敌机子弹 VS 玩家 -----
        if pygame.sprite.spritecollide(self.player, self.enemy_bullets, True):
            if self.player.hit():
                # 玩家死亡：生成大爆炸，进入游戏结束状态
                self.explosions.add(Explosion(
                    self.player.rect.centerx, self.player.rect.centery, big=True
                ))
                self.state = GAME_OVER

        # ----- 8. 碰撞检测：敌机本体 VS 玩家（相撞） -----
        if pygame.sprite.spritecollide(self.player, self.enemies, False):
            # 撞到玩家就摧毁那架敌机并产生爆炸
            for enemy in pygame.sprite.spritecollide(self.player, self.enemies, True):
                self.explosions.add(Explosion(enemy.rect.centerx, enemy.rect.centery))
            if self.player.hit():
                # 玩家死亡（无敌状态不会重复扣血）
                self.explosions.add(Explosion(
                    self.player.rect.centerx, self.player.rect.centery, big=True
                ))
                self.state = GAME_OVER

        # ----- 9. 碰撞检测：玩家拾取道具 -----
        for pu in pygame.sprite.spritecollide(self.player, self.powerups, True):
            self.player.power_up()  # 提升火力等级

        # ----- 10. 等级更新 -----
        # 每击败 20 个敌机升一级
        new_level = self.enemies_killed // 20 + 1
        if new_level > self.level:
            self.level = new_level

        # ----- 11. 连击衰减 -----
        # 每帧有 0.5% 概率连击减 1（如果不持续击落敌机）
        if self.combo > 0 and random.random() < 0.005:
            self.combo = max(0, self.combo - 1)

    def draw(self):
        """
        绘制所有画面内容。
        根据游戏状态（菜单/游戏中/结束）绘制不同的界面。
        """
        # 用背景色清空屏幕
        self.screen.fill(SKY_BLUE)

        if self.state == MENU:
            self._draw_menu()       # 绘制主菜单
        elif self.state == PLAYING:
            self._draw_game()       # 绘制游戏画面
        elif self.state == GAME_OVER:
            self._draw_game()       # 先绘制游戏画面（显示最终状态）
            self._draw_game_over()  # 再绘制半透明遮罩和结束文字

        # 刷新屏幕显示所有绘制内容
        pygame.display.flip()

    def _draw_menu(self):
        """绘制主菜单界面：星空背景 + 标题 + 操作提示"""
        # 绘制星空
        for star in self.stars:
            star.draw(self.screen)

        # 标题
        draw_text(self.screen, "飞 机 大 战", 64, SCREEN_WIDTH // 2, 180, GREEN)
        draw_text(self.screen, "Aircraft War", 36, SCREEN_WIDTH // 2, 240, GREEN)
        # 操作提示
        draw_text(self.screen, "按 空格键 或 Enter 开始游戏", 24,
                  SCREEN_WIDTH // 2, 360, YELLOW)
        draw_text(self.screen, "方向键 / WASD  移动", 20,
                  SCREEN_WIDTH // 2, 420, GRAY)
        draw_text(self.screen, "空格键  射击", 20,
                  SCREEN_WIDTH // 2, 450, GRAY)
        draw_text(self.screen, "ESC  返回菜单", 20,
                  SCREEN_WIDTH // 2, 480, GRAY)

    def _draw_game(self):
        """绘制游戏画面：星空 + 所有精灵 + HUD 信息"""
        # 绘制星空
        for star in self.stars:
            star.draw(self.screen)

        # 绘制所有精灵组（Pygame 自动按添加顺序绘制）
        self.all_sprites.draw(self.screen)
        self.bullets.draw(self.screen)
        self.enemy_bullets.draw(self.screen)
        self.explosions.draw(self.screen)
        self.powerups.draw(self.screen)
        self.player_group.draw(self.screen)

        # ---------- HUD（抬头显示） ----------
        # 左上角：分数、等级、连击数
        draw_text(self.screen, f"分数: {self.score}", 22, 10, 10, WHITE, center=False)
        draw_text(self.screen, f"等级: {self.level}", 22, 10, 36, WHITE, center=False)
        draw_text(self.screen, f"连击: x{self.combo}", 22, 10, 62, ORANGE, center=False)

        # 右上角：生命值（红色心形，空心=已损失）
        lives_text = "♥" * self.player.lives + "♡" * (3 - self.player.lives)
        draw_text(self.screen, lives_text, 24, SCREEN_WIDTH - 10, 10, RED, center=False)

        # 右上角偏下：火力等级（大于 1 时显示）
        if self.player.power > 1:
            draw_text(self.screen, f"火力 Lv.{self.player.power}", 18,
                      SCREEN_WIDTH - 10, 36, BLUE, center=False)

    def _draw_game_over(self):
        """绘制游戏结束界面：半透明遮罩 + 最终统计 + 重新开始提示"""
        # 创建半透明黑色遮罩覆盖画面（透明度 180）
        overlay = pygame.Surface((SCREEN_WIDTH, SCREEN_HEIGHT))
        overlay.set_alpha(180)
        overlay.fill(BLACK)
        self.screen.blit(overlay, (0, 0))

        # 结束文字和统计数据
        draw_text(self.screen, "游 戏 结 束", 56, SCREEN_WIDTH // 2, 220, RED)
        draw_text(self.screen, f"最终得分: {self.score}", 36,
                  SCREEN_WIDTH // 2, 300, YELLOW)
        draw_text(self.screen, f"到达等级: {self.level}", 28,
                  SCREEN_WIDTH // 2, 350, WHITE)
        draw_text(self.screen, f"击败敌机: {self.enemies_killed}", 28,
                  SCREEN_WIDTH // 2, 385, WHITE)
        # 重新开始提示
        draw_text(self.screen, "按 空格键 或 Enter 重新开始", 24,
                  SCREEN_WIDTH // 2, 460, GREEN)
        draw_text(self.screen, "按 ESC 返回菜单", 20,
                  SCREEN_WIDTH // 2, 500, GRAY)

    def run(self):
        """游戏主循环：持续处理事件→更新逻辑→绘制画面，保持 60 FPS"""
        while self.running:
            self.handle_events()  # 处理用户输入
            self.update()         # 更新游戏逻辑
            self.draw()           # 绘制画面
            self.clock.tick(FPS)  # 控制帧率，确保 60 FPS

        # 退出游戏
        pygame.quit()
        sys.exit()


# ---------------------------------------------------------------------------
# 程序入口
# ---------------------------------------------------------------------------
if __name__ == "__main__":
    # 创建游戏实例并运行
    game = Game()
    game.run()
