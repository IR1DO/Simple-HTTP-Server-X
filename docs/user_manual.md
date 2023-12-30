# 用户手册

## 1. 运行步骤

### 1.1 获取项目

`git clone https://github.com/IR1DO/Simple-HTTP-Server-X`

### 2.2 进入根目录

`cd Simple-HTTP-Server-X `

### 2.3 编译并运行

`make`

`cd build/bin`

`./httpServer`

## 2. 测试

`make test`

## 3. 查看测试覆盖率

`make coverage`

## 4. 清理测试文件

`make clean`

## 5. 注意事项

### 5.1 静态文件

静态文件并未包含在 GitHub 仓库中，用户需要手动添加。

为了处理客户端请求的静态文件，需要手动在项目的根目录下创建名为 static 的文件夹，
并将所需的静态文件放入其中。在没有明确指定请求文件的情况下，服务器将默认获取
index.html 文件。请确保 static 目录中包含所需的文件。

### 5.2 依赖工具

使用 `make coverage` 前，请确保系统中已安装以下工具：

- lcov: 用于收集代码覆盖率信息。

  - 在 Ubuntu 上安装：`sudo apt-get install lcov`

- xdg-open: 用于在默认的 Web 浏览器中打开生成的 HTML 报告。
  - 在 Ubuntu 上安装：`sudo apt-get install xdg-utils`

在其他 Linux 发行版或操作系统上，请使用相应的包管理器进行安装。
