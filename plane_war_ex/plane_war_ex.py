"""
=============================================================================
  飞机大战豪华版 - Aircraft War EX
  特性：多角色选择 | 多种弹药切换 | 绚丽爆炸特效 | 无限火力/不死角色
=============================================================================
"""
import pygame
import random
import sys
import math
import os

# ---------- Pygame 初始化 ----------
pygame.init()
pygame.mixer.init()

# ---------- 屏幕常量 ----------
SCREEN_WIDTH = 520
SCREEN_HEIGHT = 750
FPS = 60

# ---------- 颜色 ----------
BLACK    = (0, 0, 0)
WHITE    = (255, 255, 255)
RED      = (255, 30, 30)
GREEN    = (50, 255, 50)
BLUE     = (30, 100, 255)
SKY_BLUE = (20, 20, 60)
YELLOW   = (255, 240, 50)
ORANGE   = (255, 160, 20)
PURPLE   = (200, 50, 255)
CYAN     = (50, 255, 255)
PINK     = (255, 80, 180)
GRAY     = (120, 120, 120)
DARK_GRAY = (40, 40, 40)
BROWN    = (60, 40, 20)
DARK_BLUE = (5, 5, 30)
MID_BLUE  = (15, 15, 60)
LIGHT_BLUE = (30, 40, 100)
NEBULA_COLORS = [(25, 15, 50), (15, 25, 55), (35, 15, 45), (15, 30, 50)]

# ---------- 游戏状态 ----------
MENU      = 0
SELECT    = 1
PLAYING   = 2
GAME_OVER = 3

# ---------- 弹药类型 ----------
AMMO_NORMAL  = 0   # 普通弹
AMMO_DOUBLE  = 1   # 双连弹
AMMO_SCATTER = 2   # 散弹
AMMO_LASER   = 3   # 激光
AMMO_MISSILE = 4   # 导弹

AMMO_NAMES = ["普通弹", "双连弹", "散弹", "激光", "导弹"]
AMMO_COLORS = [YELLOW, ORANGE, RED, CYAN, GREEN]

# 弹药属性：[伤害, 射速延迟(ms), 弹药名]
AMMO_DATA = {
    AMMO_NORMAL:  {"damage": 1,  "delay": 160, "spread": 0, "count": 1},
    AMMO_DOUBLE:  {"damage": 1,  "delay": 150, "spread": 0, "count": 2},
    AMMO_SCATTER: {"damage": 1,  "delay": 180, "spread": 4, "count": 3},
    AMMO_LASER:   {"damage": 2,  "delay": 100, "spread": 0, "count": 1, "size": (4, 22)},
    AMMO_MISSILE: {"damage": 5,  "delay": 400, "spread": 0, "count": 1, "aoe": 50},
}

# ---------- 角色数据 ----------
CHARACTERS = [
    {
        "name": "普通飞行员",
        "desc": "均衡型，适合新手",
        "lives": 3, "speed": 6, "fire_rate": 1.0,
        "color": GREEN, "start_ammo": AMMO_NORMAL,
        "infinite_lives": False, "infinite_power": False,
    },
    {
        "name": "雷霆战机",
        "desc": "高速·五条命·双连弹起步",
        "lives": 5, "speed": 7, "fire_rate": 0.85,
        "color": CYAN, "start_ammo": AMMO_DOUBLE,
        "infinite_lives": False, "infinite_power": False,
    },
    {
        "name": "无限火力",
        "desc": "射速翻倍·弹药全满·全程狂暴",
        "lives": 3, "speed": 6, "fire_rate": 0.45,
        "color": ORANGE, "start_ammo": AMMO_SCATTER,
        "infinite_lives": False, "infinite_power": True,
    },
    {
        "name": "不死战神",
        "desc": "不死之身！尽情轰炸吧！",
        "lives": 999, "speed": 6, "fire_rate": 1.0,
        "color": PURPLE, "start_ammo": AMMO_NORMAL,
        "infinite_lives": True, "infinite_power": False,
    },
]


# ======================================================================
#  工具函数
# ======================================================================
def draw_text(surf, text, size, x, y, color=WHITE, center=True, font_name=None):
    """在指定 Surface 上绘制文字"""
    font = pygame.font.SysFont(font_name or "simhei", size)
    img = font.render(text, True, color)
    rect = img.get_rect()
    if center:
        rect.center = (x, y)
    else:
        rect.topleft = (x, y)
    surf.blit(img, rect)


def clamp(value, min_val, max_val):
    """将值限制在范围内"""
    return max(min_val, min(value, max_val))


def lerp(a, b, t):
    """线性插值"""
    return a + (b - a) * t


# ======================================================================
#  粒子系统 — 华丽爆炸特效
# ======================================================================
class Particle:
    """单个粒子：位置、速度、颜色、大小、寿命"""

    def __init__(self, x, y, color, speed, angle, size, life, gravity=0):
        self.x = x
        self.y = y
        self.vx = math.cos(angle) * speed
        self.vy = math.sin(angle) * speed
        self.color = color
        self.size = size
        self.life = life
        self.max_life = life
        self.gravity = gravity

    def update(self):
        """更新粒子位置和状态"""
        self.x += self.vx
        self.y += self.vy
        self.vy += self.gravity
        self.vx *= 0.98          # 空气阻力
        self.vy *= 0.98
        self.life -= 1

    @property
    def alive(self):
        return self.life > 0

    @property
    def alpha(self):
        """透明度随生命衰减"""
        return int(255 * (self.life / self.max_life))

    def draw(self, surf, camera_shake=(0, 0)):
        """绘制粒子（带摄像机偏移）"""
        if not self.alive:
            return
        sx = int(self.x - camera_shake[0])
        sy = int(self.y - camera_shake[1])
        alpha = self.alpha
        color = (*self.color[:3], min(alpha, 255))
        size = max(1, int(self.size * (self.life / self.max_life)))
        # 绘制带透明度的圆形粒子
        s = pygame.Surface((size * 2, size * 2), pygame.SRCALPHA)
        pygame.draw.circle(s, color, (size, size), size)
        surf.blit(s, (sx - size, sy - size))


class ExplosionEffect:
    """组合爆炸特效：多种粒子混合 + 闪光 + 冲击波"""

    def __init__(self, x, y, big=True):
        self.particles = []
        self.flash_alpha = 255 if big else 180
        self.flash_radius = 60 if big else 30
        self.shock_radius = 10
        self.shock_max = 80 if big else 40
        self.x, self.y = x, y
        self.big = big
        self._generate_particles(x, y, big)

    def _generate_particles(self, x, y, big):
        """生成各种类型的粒子"""
        count = 60 if big else 25
        colors = [YELLOW, ORANGE, RED, WHITE, (255, 100, 0)]
        if big:
            colors += [PURPLE, PINK]

        for _ in range(count):
            color = random.choice(colors)
            speed = random.uniform(2, 8) if big else random.uniform(1.5, 5)
            angle = random.uniform(0, math.pi * 2)
            size = random.uniform(2, 6) if big else random.uniform(1.5, 4)
            life = random.randint(20, 50) if big else random.randint(15, 35)
            gravity = random.uniform(0, 0.08)
            self.particles.append(Particle(x, y, color, speed, angle, size, life, gravity))

        # Boss 级爆炸：额外火花
        if big:
            for _ in range(30):
                color = (255, random.randint(150, 255), 0)
                speed = random.uniform(3, 12)
                angle = random.uniform(0, math.pi * 2)
                size = random.uniform(1, 3)
                life = random.randint(10, 25)
                self.particles.append(Particle(x, y, color, speed, angle, size, life, 0.05))

    def update(self):
        """更新所有粒子"""
        for p in self.particles:
            p.update()
        self.particles = [p for p in self.particles if p.alive]

        # 衰减闪光和冲击波
        self.flash_alpha = max(0, self.flash_alpha - 12)
        self.shock_radius += 3

    @property
    def alive(self):
        return len(self.particles) > 0 or self.flash_alpha > 0 or self.shock_radius < self.shock_max

    def draw(self, surf, camera_shake=(0, 0)):
        """绘制爆炸效果"""
        sx = int(self.x - camera_shake[0])
        sy = int(self.y - camera_shake[1])

        # 1. 冲击波（白色圆环）
        if self.shock_radius < self.shock_max:
            alpha = int(180 * (1 - self.shock_radius / self.shock_max))
            s = pygame.Surface((self.shock_max * 2, self.shock_max * 2), pygame.SRCALPHA)
            pygame.draw.circle(s, (*WHITE, alpha), (self.shock_max, self.shock_max),
                               int(self.shock_radius), 2)
            surf.blit(s, (sx - self.shock_max, sy - self.shock_max))

        # 2. 中心闪光
        if self.flash_alpha > 0:
            r = int(self.flash_radius * (self.flash_alpha / 255))
            s = pygame.Surface((r * 2, r * 2), pygame.SRCALPHA)
            for i in range(3):
                alpha = max(0, self.flash_alpha - i * 40)
                radius = int(r * (1 - i * 0.2))
                pygame.draw.circle(s, (*WHITE, alpha), (r, r), radius)
            surf.blit(s, (sx - r, sy - r))

        # 3. 粒子
        for p in self.particles:
            p.draw(surf, (0, 0))


# ======================================================================
#  华丽背景系统：渐变天空 + 闪烁星辰 + 星云 +  城市剪影
# ======================================================================
class Background:
    """多层背景：渐变天空 + 闪烁星辰 + 星云 + 城市剪影"""

    def __init__(self):
        # ---- 预先渲染渐变天空 ----
        self.sky_gradient = self._build_sky_gradient()

        # ---- 星云团 ----
        self.nebulae = []
        for _ in range(6):
            self.nebulae.append({
                "x": random.randint(0, SCREEN_WIDTH),
                "y": random.randint(0, SCREEN_HEIGHT),
                "radius": random.randint(60, 180),
                "color": random.choice(NEBULA_COLORS),
                "speed": random.uniform(0.1, 0.3),
                "phase": random.uniform(0, math.pi * 2),
            })

        # ---- 星辰 ----
        self.stars = []
        for _ in range(120):
            self.stars.append({
                "x": random.randint(0, SCREEN_WIDTH),
                "y": random.randint(0, SCREEN_HEIGHT),
                "base_bright": random.randint(80, 255),
                "speed": random.uniform(0.2, 1.5),
                "r": random.choice([1, 1, 1, 2, 2, 3]),
                "twinkle_speed": random.uniform(0.02, 0.06),
                "twinkle_phase": random.uniform(0, math.pi * 2),
            })

        # ---- 城市剪影 ----
        self.city_offset = 0
        self.city_speed = 0.5
        # 生成城市轮廓（一组矩形建筑）
        self.buildings = []
        x = 0
        while x < SCREEN_WIDTH + 200:
            w = random.randint(20, 60)
            h = random.randint(40, 140)
            self.buildings.append({
                "x": x, "w": w, "h": h,
                "windows": self._gen_windows(w, h),
            })
            x += w + random.randint(2, 8)

        # ---- 前景远山 ----
        self.mountains = []
        for _ in range(3):
            pts = []
            for i in range(0, SCREEN_WIDTH + 20, 10):
                pts.append((i, random.randint(30, 80)))
            self.mountains.append({
                "points": pts,
                "offset_y": random.randint(500, 600),
                "speed": random.uniform(0.1, 0.3),
                "color": (random.randint(15, 30), random.randint(10, 20), random.randint(25, 40)),
            })

    def _build_sky_gradient(self):
        """生成垂直渐变天空：顶部深蓝 → 底部深紫"""
        gradient = pygame.Surface((SCREEN_WIDTH, SCREEN_HEIGHT))
        for y in range(SCREEN_HEIGHT):
            t = y / SCREEN_HEIGHT
            # 顶部深蓝(5,5,40) → 中部深蓝(15,15,70) → 底部紫黑(25,10,45)
            if t < 0.5:
                t2 = t * 2
                r = int(lerp(5, 15, t2))
                g = int(lerp(5, 15, t2))
                b = int(lerp(40, 70, t2))
            else:
                t2 = (t - 0.5) * 2
                r = int(lerp(15, 25, t2))
                g = int(lerp(15, 10, t2))
                b = int(lerp(70, 45, t2))
            pygame.draw.line(gradient, (r, g, b), (0, y), (SCREEN_WIDTH, y))
        return gradient

    def _gen_windows(self, w, h):
        """生成建筑物上的窗户图案"""
        windows = []
        for wx in range(4, w - 4, 8):
            for wy in range(6, h - 10, 10):
                if random.random() < 0.6:
                    windows.append((wx, wy))
        return windows

    def update(self):
        # 星星闪烁（亮度变化）
        for s in self.stars:
            s["twinkle_phase"] += s["twinkle_speed"]

        # 星云缓慢漂移
        for n in self.nebulae:
            n["y"] += n["speed"]
            n["x"] += math.sin(n["phase"]) * 0.3
            n["phase"] += 0.01
            if n["y"] > SCREEN_HEIGHT + n["radius"]:
                n["y"] = -n["radius"]
                n["x"] = random.randint(0, SCREEN_WIDTH)

        # 城市剪影滚动
        self.city_offset += self.city_speed
        if self.city_offset > 200:
            self.city_offset = 0

        # 远山滚动
        for m in self.mountains:
            for pt in m["points"]:
                pt = (pt[0] - m["speed"], pt[1])
            # 简单处理：整体偏移
            m["offset_y"] += 0.02
            if m["offset_y"] > 620:
                m["offset_y"] = 500

    def draw(self, surf):
        # 1. 渐变天空
        surf.blit(self.sky_gradient, (0, 0))

        # 2. 星云团（半透明模糊圆）
        for n in self.nebulae:
            s = pygame.Surface((n["radius"] * 2, n["radius"] * 2), pygame.SRCALPHA)
            c = (*n["color"], 40)
            pygame.draw.circle(s, c, (n["radius"], n["radius"]), n["radius"])
            surf.blit(s, (n["x"] - n["radius"], n["y"] - n["radius"]))

        # 3. 星星（闪烁）
        for s in self.stars:
            twinkle = math.sin(s["twinkle_phase"]) * 0.3 + 0.7
            bright = int(s["base_bright"] * twinkle)
            c = (bright,) * 3
            r = max(1, int(s["r"] * twinkle))
            pygame.draw.circle(surf, c, (int(s["x"]), int(s["y"])), r)
            # 亮星加十字光晕
            if s["r"] >= 3 and twinkle > 0.8:
                glow = pygame.Surface((12, 12), pygame.SRCALPHA)
                pygame.draw.circle(glow, (*c, 30), (6, 6), 6)
                surf.blit(glow, (int(s["x"]) - 6, int(s["y"]) - 6))

        # 4. 城市剪影
        city_surf = pygame.Surface((SCREEN_WIDTH + 200, 200), pygame.SRCALPHA)
        for b in self.buildings:
            bx = b["x"] - self.city_offset
            by = 200 - b["h"]
            # 建筑主体
            pygame.draw.rect(city_surf, (10, 8, 20), (bx, by, b["w"], b["h"]))
            pygame.draw.rect(city_surf, (18, 14, 28), (bx + 1, by + 1, b["w"] - 2, b["h"] - 2))
            # 窗户
            for wx, wy in b["windows"]:
                if random.random() < 0.7:  # 部分窗户亮灯
                    win_color = (random.randint(40, 80), random.randint(40, 70), random.randint(10, 30))
                else:
                    win_color = (5, 5, 15)
                pygame.draw.rect(city_surf, win_color, (bx + wx, by + wy, 3, 4))

        # 城市剪影定位在底部
        surf.blit(city_surf, (0, SCREEN_HEIGHT - 150),
                  (0, 0, SCREEN_WIDTH, 150))

        # 5. 远山轮廓（底部隐约可见）
        for m in self.mountains:
            pts = [(x + 10 * i, m["offset_y"] + y) for i, (x, y) in enumerate(m["points"])]
            if len(pts) > 2:
                pts.append((SCREEN_WIDTH + 20, SCREEN_HEIGHT))
                pts.append((-10, SCREEN_HEIGHT))
                pygame.draw.polygon(surf, m["color"], pts)

        # 6. 底部淡淡雾气
        fog = pygame.Surface((SCREEN_WIDTH, 80), pygame.SRCALPHA)
        for y in range(80):
            alpha = int(30 * (1 - y / 80))
            pygame.draw.line(fog, (*BLACK, alpha), (0, y), (SCREEN_WIDTH, y))
        surf.blit(fog, (0, SCREEN_HEIGHT - 80))


# ======================================================================
#  玩家子弹
# ======================================================================
class Bullet(pygame.sprite.Sprite):
    """玩家子弹"""

    def __init__(self, x, y, ammo_type, vx=0, vy=-10):
        super().__init__()
        self.ammo_type = ammo_type
        self.damage = AMMO_DATA[ammo_type]["damage"]
        self.vy = vy
        self.vx = vx
        self.aoe = AMMO_DATA[ammo_type].get("aoe", 0)

        size = AMMO_DATA[ammo_type].get("size", None)
        if size:
            w, h = size
        else:
            w, h = (6, 14) if ammo_type != AMMO_MISSILE else (10, 22)

        color = AMMO_COLORS[ammo_type]

        self.image = pygame.Surface((w, h), pygame.SRCALPHA)
        if ammo_type == AMMO_LASER:
            # 激光：发光蓝色长条，带白色核心
            for i in range(3):
                alpha = 100 - i * 30
                bw = w + i * 4
                bh = h
                s = pygame.Surface((bw, bh), pygame.SRCALPHA)
                s.fill((*BLUE[:3], alpha) if i == 0 else (*CYAN[:3], alpha))
                self.image.blit(s, ((w - bw) // 2, 0))
            # 白色核心
            pygame.draw.rect(self.image, WHITE, (w//2-1, 0, 2, h))
        elif ammo_type == AMMO_MISSILE:
            # 导弹：绿色，带尾焰
            pygame.draw.rect(self.image, GREEN, (0, 0, w, h))
            pygame.draw.rect(self.image, YELLOW, (2, h-6, w-4, 6))  # 尾焰
            pygame.draw.polygon(self.image, RED, [(w//2, h), (w//2-3, h-3), (w//2+3, h-3)])
        else:
            # 普通子弹
            self.image.fill(color)
            # 发光效果
            glow = pygame.Surface((w+4, h+4), pygame.SRCALPHA)
            glow.fill((*color[:3], 60))
            self.image.blit(glow, (-2, -2))

        self.rect = self.image.get_rect(center=(x, y))

    def update(self):
        self.rect.x += self.vx
        self.rect.y += self.vy
        if self.rect.bottom < 0 or self.rect.right < 0 or self.rect.left > SCREEN_WIDTH:
            self.kill()


# ======================================================================
#  玩家飞机
# ======================================================================
class Player(pygame.sprite.Sprite):
    """玩家飞机"""

    def __init__(self, char_index):
        super().__init__()
        self.char_index = char_index
        char = CHARACTERS[char_index]
        self.base_speed = char["speed"]
        self.speed = char["speed"]
        self.lives = char["lives"]
        self.max_lives = char["lives"]
        self.infinite_lives = char["infinite_lives"]
        self.infinite_power = char["infinite_power"]
        self.fire_rate_mult = char["fire_rate"]
        self.invincible = 0

        self.ammo_type = char["start_ammo"]
        self.damage_mult = 1.0

        self.width, self.height = 44, 54
        self.image = self._create_surface(char["color"])
        self.rect = self.image.get_rect()
        self.rect.centerx = SCREEN_WIDTH // 2
        self.rect.bottom = SCREEN_HEIGHT - 40

        self.last_shot = 0

    def _create_surface(self, color):
        """绘制玩家飞机"""
        w, h = self.width, self.height
        surf = pygame.Surface((w, h), pygame.SRCALPHA)

        # 主体
        pygame.draw.polygon(surf, color, [
            (w//2, 0), (w, h//3), (w-4, h//2), (w, h-6),
            (w//2+4, h-8), (w//2, h), (w//2-4, h-8),
            (0, h-6), (4, h//2), (0, h//3),
        ])
        # 高光
        light = tuple(min(c+60, 255) for c in color[:3])
        pygame.draw.polygon(surf, light, [
            (w//2, 4), (w//2+4, h//3), (w//2-4, h//3),
        ])
        # 座舱
        pygame.draw.circle(surf, CYAN, (w//2, h//3), 6)
        pygame.draw.circle(surf, WHITE, (w//2-2, h//3-2), 2)
        # 引擎光
        pygame.draw.ellipse(surf, ORANGE, (w//2-6, h-4, 12, 4))
        return surf

    def update(self):
        if self.invincible > 0:
            self.invincible -= 1
            if self.invincible % 6 < 3:
                self.image.set_alpha(100)
            else:
                self.image.set_alpha(255)
        else:
            self.image.set_alpha(255)

    def move(self, dx, dy):
        self.rect.x += dx
        self.rect.y += dy
        self.rect.clamp_ip(pygame.Rect(0, 20, SCREEN_WIDTH, SCREEN_HEIGHT))

    def shoot(self, bullet_group):
        """根据当前弹药类型发射子弹"""
        if self.infinite_power:
            delay = int(AMMO_DATA[self.ammo_type]["delay"] * self.fire_rate_mult * 0.6)
        else:
            delay = int(AMMO_DATA[self.ammo_type]["delay"] * self.fire_rate_mult)

        now = pygame.time.get_ticks()
        if now - self.last_shot < delay:
            return
        self.last_shot = now

        data = AMMO_DATA[self.ammo_type]
        cx, cy = self.rect.centerx, self.rect.top
        count = data["count"]
        spread = data["spread"]

        bullets = []
        if self.ammo_type == AMMO_NORMAL:
            bullets.append(Bullet(cx, cy, self.ammo_type, 0, -12))
        elif self.ammo_type == AMMO_DOUBLE:
            bullets.append(Bullet(cx - 10, cy, self.ammo_type, 0, -12))
            bullets.append(Bullet(cx + 10, cy, self.ammo_type, 0, -12))
        elif self.ammo_type == AMMO_SCATTER:
            bullets.append(Bullet(cx, cy, self.ammo_type, 0, -12))
            bullets.append(Bullet(cx - 6, cy, self.ammo_type, -spread, -11))
            bullets.append(Bullet(cx + 6, cy, self.ammo_type, spread, -11))
        elif self.ammo_type == AMMO_LASER:
            bullets.append(Bullet(cx, cy - 10, self.ammo_type, 0, -18))
        elif self.ammo_type == AMMO_MISSILE:
            bullets.append(Bullet(cx - 8, cy, self.ammo_type, 0, -7))
            bullets.append(Bullet(cx + 8, cy, self.ammo_type, 0, -7))

        # 无限火力：额外多发子弹
        if self.infinite_power:
            for b in bullets[:]:
                bullets.append(Bullet(b.rect.centerx - 18, cy, self.ammo_type, -2, -10))
                bullets.append(Bullet(b.rect.centerx + 18, cy, self.ammo_type, 2, -10))
                break

        bullet_group.add(bullets)

    def hit(self):
        if self.invincible > 0:
            return False
        if self.infinite_lives:
            self.invincible = 30
            return False
        self.lives -= 1
        self.invincible = 90
        return self.lives <= 0


# ======================================================================
#  敌机子弹
# ======================================================================
class EnemyBullet(pygame.sprite.Sprite):
    def __init__(self, x, y, vx=0, vy=4):
        super().__init__()
        self.image = pygame.Surface((8, 8), pygame.SRCALPHA)
        pygame.draw.circle(self.image, RED, (4, 4), 4)
        pygame.draw.circle(self.image, (255, 150, 150), (4, 4), 2)  # 高光
        self.rect = self.image.get_rect(center=(x, y))
        self.vx, self.vy = vx, vy

    def update(self):
        self.rect.x += self.vx
        self.rect.y += self.vy
        if (self.rect.top > SCREEN_HEIGHT or self.rect.bottom < 0 or
            self.rect.right < 0 or self.rect.left > SCREEN_WIDTH):
            self.kill()


# ======================================================================
#  敌机
# ======================================================================
class Enemy(pygame.sprite.Sprite):
    """敌机基类"""

    def __init__(self, hp, speed, size, score, color, shoot_chance=0):
        super().__init__()
        self.max_hp = hp
        self.hp = hp
        self.speed = speed
        self.score = score
        self.color = color
        self.shoot_chance = shoot_chance
        self.width, self.height = size
        self.image = self._create_surface()
        self.raw_image = self.image.copy()
        self.rect = self.image.get_rect()
        self.rect.x = random.randint(0, SCREEN_WIDTH - self.width)
        self.rect.y = -self.height
        self.last_shot = 0
        self.shake_timer = 0  # 受伤震动

    def _create_surface(self):
        surf = pygame.Surface((self.width, self.height), pygame.SRCALPHA)
        return surf

    def update(self):
        self.rect.y += self.speed
        if self.rect.top > SCREEN_HEIGHT + 50:
            self.kill()
        if self.shake_timer > 0:
            self.shake_timer -= 1
            self.rect.x += random.randint(-2, 2)

    def shoot(self, bullet_group):
        if self.shoot_chance <= 0:
            return
        now = pygame.time.get_ticks()
        if now - self.last_shot < random.randint(1500, 3500):
            return
        self.last_shot = now
        if random.random() < self.shoot_chance:
            bullet_group.add(EnemyBullet(self.rect.centerx, self.rect.bottom, 0, 5))

    def hit(self, damage=1):
        self.hp -= damage
        self.shake_timer = 8
        self.image.fill(WHITE)
        return self.hp <= 0


class SmallEnemy(Enemy):
    def __init__(self, level=1):
        super().__init__(hp=1, speed=2+level*0.2, size=(32, 26),
                         score=100, color=GRAY)
        self.name = "小敌机"

    def _create_surface(self):
        surf = super()._create_surface()
        pygame.draw.ellipse(surf, GRAY, (2, 2, self.width-4, self.height-4))
        pygame.draw.ellipse(surf, (170, 170, 170), (6, 6, self.width-14, self.height//2-2))
        return surf


class NormalEnemy(Enemy):
    def __init__(self, level=1):
        super().__init__(hp=2+level//5, speed=1.5+level*0.15, size=(48, 38),
                         score=200, color=RED, shoot_chance=0.02+level*0.003)
        self.name = "普通敌机"

    def _create_surface(self):
        surf = super()._create_surface()
        pts = [(self.width//2, 0), (self.width, self.height//4),
               (self.width-4, self.height-4), (4, self.height-4),
               (0, self.height//4)]
        pygame.draw.polygon(surf, RED, pts)
        pygame.draw.circle(surf, (200, 0, 0), (self.width//2, self.height//3), 7)
        pygame.draw.circle(surf, (255, 100, 100), (self.width//2-2, self.height//3-2), 2)
        return surf


class EliteEnemy(Enemy):
    def __init__(self, level=1):
        super().__init__(hp=5+level//3, speed=1.2+level*0.1, size=(55, 45),
                         score=500, color=PURPLE, shoot_chance=0.05+level*0.005)
        self.name = "精英敌机"
        self.dir = 1

    def _create_surface(self):
        surf = super()._create_surface()
        pygame.draw.polygon(surf, PURPLE, [
            (self.width//2, 0), (self.width, 12), (self.width-3, self.height-8),
            (self.width//2+3, self.height), (self.width//2-3, self.height),
            (3, self.height-8), (0, 12)])
        pygame.draw.circle(surf, (255, 50, 50), (self.width//2, self.height//3), 8)
        pygame.draw.circle(surf, WHITE, (self.width//2-3, self.height//3-3), 2)
        return surf

    def update(self):
        super().update()
        if self.rect.y > 60:
            self.rect.x += self.dir * 2
            if self.rect.right > SCREEN_WIDTH or self.rect.left < 0:
                self.dir *= -1


class BossEnemy(Enemy):
    def __init__(self, level=1):
        boss_hp = 15 + level * 3
        super().__init__(hp=int(boss_hp), speed=0.8, size=(90, 70),
                         score=2000, color=(180, 0, 180), shoot_chance=0.08+level*0.005)
        self.name = "BOSS"
        self.entered = False
        self.enter_y = 60
        self.dir = 1

    def _create_surface(self):
        surf = super()._create_surface()
        pygame.draw.polygon(surf, (180, 0, 180), [
            (self.width//2, 0), (self.width, 18), (self.width-6, self.height-12),
            (self.width//2+6, self.height), (self.width//2-6, self.height),
            (6, self.height-12), (0, 18)])
        # 装甲
        pygame.draw.rect(surf, (220, 0, 220), (10, 20, self.width-20, 10))
        # 座舱
        pygame.draw.circle(surf, RED, (self.width//2, self.height//3), 12)
        pygame.draw.circle(surf, (255, 100, 100), (self.width//2-4, self.height//3-4), 4)
        # 武器口
        pygame.draw.circle(surf, (100, 0, 100), (12, self.height-5), 5)
        pygame.draw.circle(surf, (100, 0, 100), (self.width-12, self.height-5), 5)
        return surf

    def update(self):
        if not self.entered:
            self.rect.y += self.speed * 2
            if self.rect.y >= self.enter_y:
                self.rect.y = self.enter_y
                self.entered = True
        else:
            self.rect.x += self.dir * 1.5
            if self.rect.right > SCREEN_WIDTH or self.rect.left < 0:
                self.dir *= -1
        super().update()

    def shoot(self, bullet_group):
        if not self.entered:
            return
        now = pygame.time.get_ticks()
        if now - self.last_shot < 600:
            return
        self.last_shot = now
        bullet_group.add(EnemyBullet(self.rect.centerx, self.rect.bottom, 0, 5))
        bullet_group.add(EnemyBullet(self.rect.centerx-20, self.rect.bottom-5, -1.5, 5))
        bullet_group.add(EnemyBullet(self.rect.centerx+20, self.rect.bottom-5, 1.5, 5))

    def hit(self, damage=1):
        self.hp -= damage
        self.shake_timer = 6
        self.image.fill((255, 0, 255))
        return self.hp <= 0


# ======================================================================
#  弹药掉落道具
# ======================================================================
class AmmoPickup(pygame.sprite.Sprite):
    """弹药掉落道具"""

    def __init__(self, x, y, ammo_type):
        super().__init__()
        self.ammo_type = ammo_type
        self.size = 24
        self.image = pygame.Surface((self.size, self.size), pygame.SRCALPHA)
        color = AMMO_COLORS[ammo_type]

        # 外圈
        pygame.draw.circle(self.image, color, (self.size//2, self.size//2), self.size//2)
        pygame.draw.circle(self.image, WHITE, (self.size//2, self.size//2), self.size//2-3)
        # 图标
        icons = {AMMO_NORMAL: "N", AMMO_DOUBLE: "D", AMMO_SCATTER: "S",
                 AMMO_LASER: "L", AMMO_MISSILE: "M"}
        draw_text(self.image, icons[ammo_type], 16, self.size//2, self.size//2, color)

        self.rect = self.image.get_rect(center=(x, y))
        self.speed = 1.5

    def update(self):
        self.rect.y += self.speed
        if self.rect.top > SCREEN_HEIGHT:
            self.kill()


# ======================================================================
#  游戏主类
# ======================================================================
class Game:
    def __init__(self):
        self.screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT))
        pygame.display.set_caption("飞机大战 EX")
        self.clock = pygame.time.Clock()
        self.running = True
        self.state = MENU

        self.stars = Background()
        self.selected_char = 0

        # 游戏实体
        self.player = None
        self.all_sprites = None
        self.bullets = None
        self.enemies = None
        self.enemy_bullets = None
        self.explosions = []
        self.powerups = None

        # 游戏统计
        self.score = 0
        self.level = 1
        self.enemies_killed = 0
        self.combo = 0
        self.combo_timer = 0
        self.spawn_timer = 0
        self.boss_spawned = False
        self.shake_amount = 0  # 屏幕震动

        # 射击特效
        self.muzzle_flashes = []

    def reset(self):
        """重置游戏"""
        char = CHARACTERS[self.selected_char]
        self.player = Player(self.selected_char)
        self.all_sprites = pygame.sprite.Group()
        self.bullets = pygame.sprite.Group()
        self.enemies = pygame.sprite.Group()
        self.enemy_bullets = pygame.sprite.Group()
        self.powerups = pygame.sprite.Group()
        self.explosions.clear()
        self.muzzle_flashes.clear()

        self.score = 0
        self.level = 1
        self.enemies_killed = 0
        self.combo = 0
        self.combo_timer = 0
        self.spawn_timer = 0
        self.boss_spawned = False
        self.shake_amount = 0

    def spawn_enemy(self):
        """生成敌机"""
        r = random.random()
        if self.enemies_killed > 0 and self.enemies_killed % 40 == 0 and not self.boss_spawned:
            enemy = BossEnemy(self.level)
            self.boss_spawned = True
        elif self.level >= 5 and r < 0.15:
            enemy = EliteEnemy(self.level)
        elif r < 0.5:
            enemy = SmallEnemy(self.level)
        else:
            enemy = NormalEnemy(self.level)

        self.enemies.add(enemy)
        self.all_sprites.add(enemy)

    def add_explosion(self, x, y, big=False):
        """添加爆炸特效"""
        self.explosions.append(ExplosionEffect(x, y, big))
        if big:
            self.shake_amount = 12  # 大爆炸屏幕震动

    def spawn_ammo_drop(self, x, y):
        """掉落弹药道具"""
        # 不掉落当前使用的弹药，或者一定概率
        ammo_types = [t for t in range(5) if t != self.player.ammo_type]
        if ammo_types and random.random() < 0.25:
            at = random.choice(ammo_types)
            self.powerups.add(AmmoPickup(x, y, at))

    # ---- 事件处理 ----
    def handle_events(self):
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                self.running = False
                return

            if event.type == pygame.KEYDOWN:
                if self.state == MENU:
                    if event.key in (pygame.K_RETURN, pygame.K_SPACE):
                        self.state = SELECT
                elif self.state == SELECT:
                    if event.key == pygame.K_LEFT:
                        self.selected_char = (self.selected_char - 1) % len(CHARACTERS)
                    elif event.key == pygame.K_RIGHT:
                        self.selected_char = (self.selected_char + 1) % len(CHARACTERS)
                    elif event.key in (pygame.K_RETURN, pygame.K_SPACE):
                        self.state = PLAYING
                        self.reset()
                elif self.state == GAME_OVER:
                    if event.key in (pygame.K_RETURN, pygame.K_SPACE):
                        self.state = MENU

    # ---- 更新逻辑 ----
    def update(self):
        if self.state in (MENU, SELECT):
            self.stars.update()
            return

        if self.state != PLAYING:
            self.stars.update()
            self._update_explosions()
            return

        # 1. 星空
        self.stars.update()

        # 2. 屏幕震动衰减
        if self.shake_amount > 0:
            self.shake_amount *= 0.85
            if self.shake_amount < 0.5:
                self.shake_amount = 0

        # 3. 玩家输入
        keys = pygame.key.get_pressed()
        dx = dy = 0
        if keys[pygame.K_LEFT] or keys[pygame.K_a]:
            dx = -self.player.speed
        if keys[pygame.K_RIGHT] or keys[pygame.K_d]:
            dx = self.player.speed
        if keys[pygame.K_UP] or keys[pygame.K_w]:
            dy = -self.player.speed
        if keys[pygame.K_DOWN] or keys[pygame.K_s]:
            dy = self.player.speed
        self.player.move(dx, dy)

        # 射击
        if keys[pygame.K_SPACE]:
            self.player.shoot(self.bullets)

        # 切换弹药（按 Q 键）
        if keys[pygame.K_q] and pygame.time.get_ticks() % 20 == 0:
            self.player.ammo_type = (self.player.ammo_type + 1) % 5

        # 4. 更新所有精灵
        self.player.update()
        self.bullets.update()
        self.enemies.update()
        self.enemy_bullets.update()
        self.powerups.update()

        # 5. 敌机射击
        for e in self.enemies:
            e.shoot(self.enemy_bullets)

        # 6. 生成敌机
        self.spawn_timer += 1
        spawn_rate = max(12, 55 - self.level * 3)
        if self.spawn_timer >= spawn_rate:
            self.spawn_timer = 0
            self.spawn_enemy()

        # 7. 子弹 vs 敌机
        hits = pygame.sprite.groupcollide(self.bullets, self.enemies, True, False)
        for bullet, enemy_list in hits.items():
            for enemy in enemy_list:
                damage = bullet.damage
                if enemy.hit(damage):
                    self.score += enemy.score * (1 + self.combo // 10)
                    self.enemies_killed += 1
                    self.combo += 1
                    self.combo_timer = 120
                    is_boss = isinstance(enemy, BossEnemy)
                    self.add_explosion(enemy.rect.centerx, enemy.rect.centery, big=is_boss)
                    self.spawn_ammo_drop(enemy.rect.centerx, enemy.rect.centery)
                    if is_boss:
                        self.boss_spawned = False
                    enemy.kill()

                    # 导弹 AOE 伤害
                    if bullet.aoe > 0:
                        for other in list(self.enemies):
                            dx = other.rect.centerx - enemy.rect.centerx
                            dy = other.rect.centery - enemy.rect.centery
                            if math.hypot(dx, dy) < bullet.aoe:
                                if other.hit(2):
                                    self.score += other.score
                                    self.enemies_killed += 1
                                    self.add_explosion(other.rect.centerx, other.rect.centery)
                                    other.kill()

        # 8. 敌机子弹 vs 玩家
        if pygame.sprite.spritecollide(self.player, self.enemy_bullets, True):
            if self.player.hit():
                self.add_explosion(self.player.rect.centerx, self.player.rect.centery, big=True)
                if not self.player.infinite_lives:
                    self.state = GAME_OVER

        # 9. 敌机撞玩家
        if pygame.sprite.spritecollide(self.player, self.enemies, True):
            self.add_explosion(self.player.rect.centerx, self.player.rect.centery)
            if self.player.hit():
                self.add_explosion(self.player.rect.centerx, self.player.rect.centery, big=True)
                if not self.player.infinite_lives:
                    self.state = GAME_OVER

        # 10. 拾取弹药
        for pu in pygame.sprite.spritecollide(self.player, self.powerups, True):
            self.player.ammo_type = pu.ammo_type
            # 拾取闪光
            self.muzzle_flashes.append({
                "x": self.player.rect.centerx, "y": self.player.rect.centery,
                "alpha": 255, "radius": 20,
            })

        # 11. 更新特效
        self._update_explosions()
        self._update_muzzle_flashes()

        # 12. 连击计时
        if self.combo_timer > 0:
            self.combo_timer -= 1
            if self.combo_timer == 0:
                self.combo = 0

        # 13. 等级
        self.level = self.enemies_killed // 15 + 1

    def _update_explosions(self):
        """更新所有爆炸特效"""
        for exp in self.explosions[:]:
            exp.update()
            if not exp.alive:
                self.explosions.remove(exp)

    def _update_muzzle_flashes(self):
        """更新拾取闪光"""
        for f in self.muzzle_flashes[:]:
            f["alpha"] -= 15
            f["radius"] += 2
            if f["alpha"] <= 0:
                self.muzzle_flashes.remove(f)

    # ---- 绘制 ----
    def draw(self):
        # 计算屏幕震动偏移
        sx = random.randint(-int(self.shake_amount), int(self.shake_amount)) if self.shake_amount > 0.5 else 0
        sy = random.randint(-int(self.shake_amount), int(self.shake_amount)) if self.shake_amount > 0.5 else 0

        # 绘制华丽的背景（渐变天空 + 星辰 + 星云 + 城市剪影）
        self.stars.draw(self.screen)

        # 通知子绘制函数震动偏移
        shake = (sx, sy)

        if self.state == MENU:
            self._draw_menu(shake)
        elif self.state == SELECT:
            self._draw_select(shake)
        elif self.state == PLAYING:
            self._draw_playing(shake)
        elif self.state == GAME_OVER:
            self._draw_playing(shake)
            self._draw_game_over()

        pygame.display.flip()

    def _draw_menu(self, shake):
        dx, dy = shake

        draw_text(self.screen, "飞 机 大 战 EX", 56, SCREEN_WIDTH//2+dx, 140+dy, GREEN)
        draw_text(self.screen, "AIRCRAFT WAR EX", 28, SCREEN_WIDTH//2+dx, 195+dy, CYAN)
        draw_text(self.screen, "按 空格键 进入角色选择", 24, SCREEN_WIDTH//2+dx, 320+dy, YELLOW)
        draw_text(self.screen, "方向键 / WASD  移动", 20, SCREEN_WIDTH//2+dx, 400+dy, GRAY)
        draw_text(self.screen, "空格键  射击   Q键  切换弹药", 20, SCREEN_WIDTH//2+dx, 430+dy, GRAY)

        # 角色预览
        for i, ch in enumerate(CHARACTERS):
            y_pos = 530 + i * 40
            color = ch["color"]
            pygame.draw.rect(self.screen, color, (SCREEN_WIDTH//2-160+dx, y_pos-10+dy, 320, 34), 2)
            draw_text(self.screen, f"{ch['name']:>8}  |  {ch['desc']}", 16,
                      SCREEN_WIDTH//2+dx, y_pos+dy, color)

    def _draw_select(self, shake):
        dx, dy = shake
        ch = CHARACTERS[self.selected_char]

        draw_text(self.screen, "选 择 角 色", 48, SCREEN_WIDTH//2+dx, 60+dy, WHITE)
        draw_text(self.screen, "←  /  →  切换", 20, SCREEN_WIDTH//2+dx, 110+dy, GRAY)

        # 角色框
        box_w, box_h = 360, 280
        bx, by = SCREEN_WIDTH//2 - box_w//2, 160
        pygame.draw.rect(self.screen, ch["color"], (bx+dx, by+dy, box_w, box_h), 3)
        pygame.draw.rect(self.screen, DARK_GRAY, (bx+dx+6, by+dy+6, box_w-12, box_h-12))

        # 角色名
        draw_text(self.screen, ch["name"], 40, SCREEN_WIDTH//2+dx, by+70+dy, ch["color"])
        # 描述
        draw_text(self.screen, ch["desc"], 20, SCREEN_WIDTH//2+dx, by+120+dy, WHITE)
        # 属性
        stats = f"速度: {ch['speed']}  |  生命: {'∞' if ch['infinite_lives'] else ch['lives']}"
        draw_text(self.screen, stats, 18, SCREEN_WIDTH//2+dx, by+170+dy, GRAY)
        stats2 = f"弹药: {AMMO_NAMES[ch['start_ammo']]}  |  无限火力: {'是' if ch['infinite_power'] else '否'}"
        draw_text(self.screen, stats2, 18, SCREEN_WIDTH//2+dx, by+200+dy, GRAY)

        draw_text(self.screen, "按 空格键 / Enter 开始游戏", 22,
                  SCREEN_WIDTH//2+dx, 500+dy, YELLOW)

    def _draw_playing(self, shake):
        dx, dy = shake

        # 所有精灵
        self.all_sprites.draw(self.screen)
        self.bullets.draw(self.screen)
        self.enemy_bullets.draw(self.screen)
        self.powerups.draw(self.screen)
        self.screen.blit(self.player.image,
                         (self.player.rect.x + dx, self.player.rect.y + dy))

        # 爆炸特效（在精灵之上）
        for exp in self.explosions:
            exp.draw(self.screen, shake)

        # 拾取闪光
        for f in self.muzzle_flashes:
            s = pygame.Surface((f["radius"]*2, f["radius"]*2), pygame.SRCALPHA)
            pygame.draw.circle(s, (*CYAN, f["alpha"]), (f["radius"], f["radius"]), f["radius"])
            self.screen.blit(s, (f["x"]+dx-f["radius"], f["y"]+dy-f["radius"]))

        # HUD
        self._draw_hud()

    def _draw_hud(self):
        """抬头显示"""
        # 左上角
        draw_text(self.screen, f"分数: {self.score}", 22, 15, 12, WHITE, center=False)
        draw_text(self.screen, f"等级: {self.level}", 20, 15, 40, WHITE, center=False)
        draw_text(self.screen, f"击毁: {self.enemies_killed}", 18, 15, 66, GRAY, center=False)

        # 连击
        if self.combo > 1:
            draw_text(self.screen, f"连击 x{self.combo}", 24, 80, 96, ORANGE, center=False)

        # 右上角：生命
        ch = CHARACTERS[self.selected_char]
        if ch["infinite_lives"]:
            lives_str = "∞ 不死"
        else:
            hearts = "♥" * self.player.lives + "♡" * (ch["lives"] - self.player.lives)
            lives_str = hearts
        draw_text(self.screen, lives_str, 22, SCREEN_WIDTH-15, 12, RED, center=False)

        # 弹药显示
        ammo_color = AMMO_COLORS[self.player.ammo_type]
        draw_text(self.screen, f"弹药: {AMMO_NAMES[self.player.ammo_type]}", 20,
                  SCREEN_WIDTH-15, 38, ammo_color, center=False)

        # 弹药伤害
        dmg = AMMO_DATA[self.player.ammo_type]["damage"]
        draw_text(self.screen, f"伤害: {dmg}", 16,
                  SCREEN_WIDTH-15, 60, GRAY, center=False)

        # 切换提示
        draw_text(self.screen, "Q 切换弹药", 14, SCREEN_WIDTH-15, 82, GRAY, center=False)

    def _draw_game_over(self):
        """游戏结束画面"""
        overlay = pygame.Surface((SCREEN_WIDTH, SCREEN_HEIGHT))
        overlay.set_alpha(160)
        overlay.fill(BLACK)
        self.screen.blit(overlay, (0, 0))

        draw_text(self.screen, "游 戏 结 束", 52, SCREEN_WIDTH//2, 200, RED)
        draw_text(self.screen, f"最终得分: {self.score}", 36,
                  SCREEN_WIDTH//2, 280, YELLOW)
        draw_text(self.screen, f"到达等级: {self.level}", 24,
                  SCREEN_WIDTH//2, 330, WHITE)
        draw_text(self.screen, f"击败敌机: {self.enemies_killed}", 24,
                  SCREEN_WIDTH//2, 365, WHITE)
        draw_text(self.screen, f"角色: {CHARACTERS[self.selected_char]['name']}", 22,
                  SCREEN_WIDTH//2, 410, GRAY)

        draw_text(self.screen, "按 空格键 / Enter 返回菜单", 24,
                  SCREEN_WIDTH//2, 490, GREEN)

    # ---- 主循环 ----
    def run(self):
        while self.running:
            self.handle_events()
            self.update()
            self.draw()
            self.clock.tick(FPS)

        pygame.quit()
        sys.exit()


# ======================================================================
#  程序入口
# ======================================================================
if __name__ == "__main__":
    game = Game()
    game.run()
