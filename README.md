# QBlueprint(Qt5 Qt6兼容)
 - [访问我的bilibili个人主页查看使用方法](https://space.bilibili.com/286896507?spm_id_from=333.1007.0.0)
 - [关注我的Github](https://github.com/IgnorAnsel)
   
`QBlueprint` 是一个基于 Qt 实现的蓝图系统，类似于 Unreal Engine 中的蓝图系统。它允许用户通过节点连接的方式构建逻辑流，并支持通过函数创建节点。

## 功能介绍

### 1. **蓝图节点连接**
   - 支持将蓝图节点通过输入、输出端口进行连接，形成逻辑流。
   - 通过拖拽操作，用户可以直观地连接不同节点，构建节点网络。

### 2. **通过函数创建蓝图节点**
   - 支持根据函数自动生成蓝图节点。
   - 用户可以通过特定的函数自动生成对应的蓝图节点，并在蓝图系统中使用。

## 使用方法

### 1. 克隆项目

```bash
git clone https://github.com/IgnorAnsel/QBlueprint.git
cd QBlueprint
```
### 2. 构建项目
#### 使用 CMake 构建
确保你已经安装了 Qt 和 CMake。可以通过以下步骤来构建项目：
```bash
mkdir build
cd build
cmake ..
cmake --build .
```
### 3. 运行项目
构建完成后，你可以在 build 目录下找到生成的可执行文件，直接运行它来启动 QBlueprint 应用程序：

```bash
./QBlueprint
```
### 4. 创建节点并连接
   - 启动程序后，使用右键菜单创建蓝图节点。 
   - 选择 int 类型的节点，拖拽以创建输入和输出端口。
   - 通过拖拽端口进行连接，实现节点间的逻辑流。
   
## 未来计划
- 补全更多的数据类型。
- 提供复杂的逻辑控制功能，如条件判断和循环控制节点。
- 实现蓝图执行功能，支持运行时逻辑计算。

## 贡献指南
- 我们欢迎社区贡献！以下是贡献的步骤：

- Fork 此仓库
- 创建一个新的分支 (git checkout -b feature/YourFeature)
- 提交更改 (git commit -m 'Add some feature')
- 推送到你自己的分支 (git push origin feature/YourFeature)
- 打开一个 Pull Request

## 许可协议
本项目遵循 MIT 许可协议。
