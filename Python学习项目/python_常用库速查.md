# Python 常用库速查手册

> 按分类整理的 Python 常用第三方库及标准库，含用途说明和安装命令。
> 更新日期：2026年5月

---

## 一、Web 开发

| 库名 | 用途 | 安装/来源 |
|------|------|----------|
| **requests** | HTTP 请求库，发送 GET/POST 请求，处理 API 调用 | `pip install requests` |
| **Flask** | 轻量级 Web 框架，适合小型项目和 API 服务 | `pip install flask` |
| **FastAPI** | 现代高性能 Web 框架，自动生成 API 文档，支持异步 | `pip install fastapi uvicorn` |
| **Django** | 全栈 Web 框架，自带 ORM/Admin/认证 | `pip install django` |
| **aiohttp** | 异步 HTTP 客户端/服务器框架 | `pip install aiohttp` |
| **httpx** | 下一代 HTTP 客户端，支持 HTTP/2 和异步 | `pip install httpx` |
| **uvicorn** | ASGI 服务器，运行 FastAPI 等异步框架 | `pip install uvicorn` |
| **gunicorn** | WSGI HTTP 服务器，生产环境部署 Flask/Django | `pip install gunicorn` |

---

## 二、数据科学与数值计算

| 库名 | 用途 | 安装/来源 |
|------|------|----------|
| **numpy** | 数值计算核心库，多维数组和矩阵运算（所有数据科学库的基石） | `pip install numpy` |
| **pandas** | 数据分析库，DataFrame 表格数据处理、清洗、聚合 | `pip install pandas` |
| **scipy** | 科学计算库，线性代数/优化/信号处理/统计 | `pip install scipy` |
| **polars** | 新一代高性能 DataFrame 库（Rust 实现，比 pandas 快很多） | `pip install polars` |
| **dask** | 并行计算库，处理超出内存的大数据集 | `pip install dask` |

---

## 三、数据可视化

| 库名 | 用途 | 安装/来源 |
|------|------|----------|
| **matplotlib** | 最基础的绘图库，功能全面但语法较底层 | `pip install matplotlib` |
| **seaborn** | 基于 matplotlib 的统计可视化，默认样式更好看 | `pip install seaborn` |
| **plotly** | 交互式图表，支持 Web 端缩放/悬停等 | `pip install plotly` |
| **pyecharts** | 国产交互图表库（基于 ECharts），中文支持好 | `pip install pyecharts` |
| **streamlit** | 纯 Python 写数据仪表盘/Web 应用，无需前端知识 | `pip install streamlit` |
| **dash** | Plotly 出品的仪表盘框架，企业级数据应用 | `pip install dash` |

---

## 四、人工智能 / 机器学习

| 库名 | 用途 | 安装/来源 |
|------|------|----------|
| **scikit-learn** | 经典机器学习库，分类/回归/聚类/降维等 | `pip install scikit-learn` |
| **pytorch** | 深度学习框架（Meta），动态计算图，研究界首选 | `pip install torch` |
| **tensorflow** | 深度学习框架（Google），生产部署成熟 | `pip install tensorflow` |
| **transformers** | Hugging Face 出品，预训练大模型加载与微调 | `pip install transformers` |
| **keras** | 高级神经网络 API，作为 TensorFlow 的一部分 | `pip install keras` |
| **xgboost** | 梯度提升树，结构化数据上的王者算法 | `pip install xgboost` |
| **lightgbm** | 微软出品梯度提升树，速度比 xgboost 快 | `pip install lightgbm` |
| **langchain** | LLM 应用开发框架，链式调用/Agent/工具集成 | `pip install langchain` |
| **llama-index** | LLM 数据框架，专注于 RAG 和知识检索 | `pip install llama-index` |
| **openai** | OpenAI API 官方客户端（GPT-4/ChatGPT 等） | `pip install openai` |
| **anthropic** | Anthropic API 官方客户端（Claude 等） | `pip install anthropic` |

---

## 五、爬虫与自动化

| 库名 | 用途 | 安装/来源 |
|------|------|----------|
| **beautifulsoup4** | HTML/XML 解析器，配合 requests 做爬虫 | `pip install beautifulsoup4` |
| **scrapy** | 功能完整的企业级爬虫框架 | `pip install scrapy` |
| **selenium** | 浏览器自动化（模拟真实用户点击/输入） | `pip install selenium` |
| **playwright** | 微软出品，新一代浏览器自动化，比 selenium 更快更稳 | `pip install playwright` |
| **lxml** | 高性能 XML/HTML 解析库（C 语言实现） | `pip install lxml` |
| **pyautogui** | 桌面 GUI 自动化，模拟键盘鼠标操作 | `pip install pyautogui` |
| **requests-html** | requests 作者出品，集成 JS 渲染的爬虫工具 | `pip install requests-html` |

---

## 六、办公文档处理

| 库名 | 用途 | 安装/来源 |
|------|------|----------|
| **python-docx** | 生成和修改 Word(.docx) 文档 | `pip install python-docx` |
| **openpyxl** | 生成和修改 Excel(.xlsx) 文档 | `pip install openpyxl` |
| **python-pptx** | 生成和修改 PowerPoint(.pptx) 文档 | `pip install python-pptx` |
| **pypdf** | PDF 文件读取/合并/分割 | `pip install pypdf` |
| **reportlab** | 编程方式生成 PDF 文档 | `pip install reportlab` |
| **tabula-py** | 从 PDF 中提取表格数据 | `pip install tabula-py` |

---

## 七、数据库

| 库名 | 用途 | 安装/来源 |
|------|------|----------|
| **sqlite3** | 轻量级嵌入式数据库（标准库，无需安装） | 内置 |
| **sqlalchemy** | Python SQL 工具包和 ORM，支持多种数据库 | `pip install sqlalchemy` |
| **psycopg2** | PostgreSQL 数据库驱动 | `pip install psycopg2` |
| **pymysql** | MySQL 数据库驱动 | `pip install pymysql` |
| **pymongo** | MongoDB 驱动 | `pip install pymongo` |
| **redis** | Redis 客户端（缓存/队列/发布订阅） | `pip install redis` |

---

## 八、图像与多媒体

| 库名 | 用途 | 安装/来源 |
|------|------|----------|
| **pillow (PIL)** | Python 图像处理标准库，打开/修改/保存各种格式图片 | `pip install pillow` |
| **opencv-python** | 计算机视觉库，图像识别/视频处理/人脸检测 | `pip install opencv-python` |
| **moviepy** | 视频编辑库，剪辑/合并/加字幕/特效 | `pip install moviepy` |
| **pygame** | 2D 游戏开发库 | `pip install pygame` |

---

## 九、GUI 桌面应用

| 库名 | 用途 | 安装/来源 |
|------|------|----------|
| **tkinter** | Python 自带 GUI 库，简单够用（标准库） | 内置 |
| **PyQt6 / PySide6** | Qt 框架的 Python 绑定，专业级 GUI | `pip install PyQt6` |
| **customtkinter** | tkinter 美化增强版，现代风格 | `pip install customtkinter` |
| **flet** | 基于 Flutter 的 Python GUI，一套代码多平台运行 | `pip install flet` |

---

## 十、网络与通信

| 库名 | 用途 | 安装/来源 |
|------|------|----------|
| **socket** | 底层网络编程接口（标准库） | 内置 |
| **paramiko** | SSH 客户端，远程执行命令/传输文件 | `pip install paramiko` |
| **websockets** | WebSocket 客户端/服务器 | `pip install websockets` |
| **paho-mqtt** | MQTT 物联网协议客户端 | `pip install paho-mqtt` |

---

## 十一、实用工具

| 库名 | 用途 | 安装/来源 |
|------|------|----------|
| **argparse** | 命令行参数解析（标准库） | 内置 |
| **click** | 命令行工具框架，比 argparse 更简洁 | `pip install click` |
| **typer** | 基于类型提示的 CLI 框架，自动生成帮助文档 | `pip install typer` |
| **tqdm** | 进度条库，循环迭代时显示进度 | `pip install tqdm` |
| **pydantic** | 数据校验和设置管理（FastAPI 的基础） | `pip install pydantic` |
| **python-dotenv** | 从 .env 文件加载环境变量 | `pip install python-dotenv` |
| **rich** | 终端美化输出，彩色文本/表格/进度条/Markdown | `pip install rich` |
| **pyinstaller** | 将 Python 程序打包为独立可执行文件(.exe) | `pip install pyinstaller` |
| **black** | Python 代码格式化工具（零配置） | `pip install black` |
| **ruff** | 超快 Python linter+formatter（Rust 实现） | `pip install ruff` |
| **pytest** | 测试框架（比 unittest 更简洁，社区首选） | `pip install pytest` |
| **pytest-cov** | 测试覆盖率报告 | `pip install pytest-cov` |
| **mypy** | 静态类型检查工具 | `pip install mypy` |
| **pre-commit** | Git 钩子管理，提交前自动检查 | `pip install pre-commit` |
| **uv** | Rust 实现的高速 Python 包管理器（替代 pip） | `pip install uv` |

---

## 十二、Python 常用标准库速览

以下库随 Python 一起安装，无需额外 pip：

| 标准库 | 用途 |
|--------|------|
| **os** | 操作系统接口（文件/目录/进程/环境变量） |
| **sys** | Python 解释器相关（命令行参数/退出/路径） |
| **math** | 数学函数（三角函数/对数/PI 等） |
| **random** | 随机数生成 |
| **datetime** | 日期时间处理 |
| **json** | JSON 编解码 |
| **csv** | CSV 文件读写 |
| **re** | 正则表达式 |
| **pathlib** | 面向对象的文件路径操作（推荐替代 os.path） |
| **collections** | 高级容器数据类型（Counter/OrderedDict/deque 等） |
| **itertools** | 迭代器工具函数（排列组合/无限迭代器等） |
| **functools** | 高阶函数工具（reduce/lru_cache/wraps 等） |
| **typing** | 类型提示支持 |
| **logging** | 日志框架 |
| **unittest** | 单元测试框架 |
| **threading** | 多线程 |
| **multiprocessing** | 多进程 |
| **asyncio** | 异步 I/O（协程） |
| **subprocess** | 运行子进程（调用外部命令） |
| **shutil** | 高级文件操作（复制/删除目录树等） |
| **hashlib** | 哈希算法（MD5/SHA256 等） |
| **base64** | Base64 编解码 |
| **urllib** | URL 处理（请求/解析） |
| **xml** | XML 解析 |
| **smtplib** | 发送邮件（SMTP） |
| **sqlite3** | SQLite 数据库 |
| **configparser** | 配置文件(.ini)读写 |
| **argparse** | 命令行参数解析 |
| **tempfile** | 临时文件和目录 |
| **zipfile / tarfile** | 压缩文件操作 |

---

> **提示：** 安装第三方库前建议先创建虚拟环境：
> ```bash
> python -m venv .venv
> source .venv/bin/activate  # Linux/Mac
> .venv\Scripts\activate     # Windows
> ```
