# 需求文档

## 简介

Power Window Anti-Pinch Controller 是一个符合 AUTOSAR 架构风格的车窗控制应用软件组件（ASW/SWC）。该系统实现车窗的手动/自动升降控制、防夹保护、故障诊断、非易失性存储以及标定参数管理功能，确保车窗操作的安全性和可靠性。

## 术语表

- **PowerWindow_System**: 车窗控制系统，负责处理车窗升降控制逻辑
- **Safety_Monitor**: 安全监控模块，负责防夹检测和保护
- **State_Machine**: 状态机模块，管理车窗运行状态转换
- **NvM_Manager**: 非易失性内存管理器，负责数据持久化
- **Diag_Interface**: 诊断接口模块，提供故障诊断和状态查询
- **Calibration_Parameter**: 标定参数，可配置的系统参数
- **Anti_Pinch**: 防夹功能，检测夹物并执行保护动作
- **Position_Sensor**: 位置传感器，提供车窗当前位置信息
- **Current_Sensor**: 电流传感器，监测电机电流
- **DTC**: 诊断故障码（Diagnostic Trouble Code）
- **Runnable**: AUTOSAR 可调度实体，周期性执行的函数

## 需求

### 需求 1: 手动车窗控制

**用户故事:** 作为驾驶员，我希望能够手动控制车窗升降和停止，以便根据需要调节车窗位置。

#### 验收标准

1. WHEN 用户按下升窗按钮，THE PowerWindow_System SHALL 驱动车窗向上移动
2. WHEN 用户按下降窗按钮，THE PowerWindow_System SHALL 驱动车窗向下移动
3. WHEN 用户释放升降按钮，THE PowerWindow_System SHALL 停止车窗移动
4. WHILE 车窗正在手动移动，THE PowerWindow_System SHALL 每 10ms 更新车窗位置
5. IF 车窗到达上限位置，THEN THE PowerWindow_System SHALL 停止向上移动并保持位置
6. IF 车窗到达下限位置，THEN THE PowerWindow_System SHALL 停止向下移动并保持位置

### 需求 2: 自动车窗控制

**用户故事:** 作为驾驶员，我希望能够一键自动升降车窗，以便快速操作而无需持续按压按钮。

#### 验收标准

1. WHEN 用户短按升窗按钮超过标定时间阈值，THE PowerWindow_System SHALL 自动升窗至完全关闭位置
2. WHEN 用户短按降窗按钮超过标定时间阈值，THE PowerWindow_System SHALL 自动降窗至完全打开位置
3. WHILE 车窗处于自动升降模式，THE PowerWindow_System SHALL 持续驱动车窗直至到达目标位置
4. WHEN 用户在自动模式中再次按下任意按钮，THE PowerWindow_System SHALL 立即停止自动移动
5. WHEN 自动升降完成，THE PowerWindow_System SHALL 转换至空闲状态

### 需求 3: 防夹保护功能

**用户故事:** 作为驾驶员，我希望车窗在升窗过程中能够检测到夹物并自动回退，以保护乘客和物品安全。

#### 验收标准

1. WHILE 车窗正在向上移动，THE Safety_Monitor SHALL 每 10ms 监测电机电流值
2. IF 电机电流超过防夹阈值标定参数，THEN THE Safety_Monitor SHALL 触发防夹事件
3. WHEN 防夹事件被触发，THE PowerWindow_System SHALL 立即停止向上移动
4. WHEN 防夹事件被触发，THE PowerWindow_System SHALL 自动向下移动标定的回退距离
5. WHEN 回退完成后，THE PowerWindow_System SHALL 转换至空闲状态并禁止自动升窗 5 秒
6. WHERE 防夹功能已启用，THE Safety_Monitor SHALL 记录防夹事件到诊断日志

### 需求 4: 位置传感器管理

**用户故事:** 作为系统，我需要准确获取车窗位置信息，以便实现精确的位置控制和防夹检测。

#### 验收标准

1. THE PowerWindow_System SHALL 每 10ms 读取 Position_Sensor 的原始值
2. THE PowerWindow_System SHALL 将传感器原始值转换为百分比位置（0-100%）
3. IF Position_Sensor 读取值超出有效范围，THEN THE PowerWindow_System SHALL 设置位置传感器故障标志
4. WHEN 位置传感器故障被检测到，THE PowerWindow_System SHALL 禁止所有车窗移动操作
5. THE PowerWindow_System SHALL 将当前位置值存储至 NvM_Manager 每 1 秒或状态变化时

### 需求 5: 电流监测与故障检测

**用户故事:** 作为系统，我需要监测电机电流异常情况，以便及时发现故障并保护硬件。

#### 验收标准

1. WHILE 车窗正在移动，THE PowerWindow_System SHALL 每 10ms 读取 Current_Sensor 值
2. IF 电流值超过过流阈值标定参数持续超过 100ms，THEN THE PowerWindow_System SHALL 设置过流故障码
3. WHEN 过流故障被检测到，THE PowerWindow_System SHALL 立即停止车窗移动并进入故障状态
4. IF 电流值低于欠流阈值标定参数持续超过 100ms，THEN THE PowerWindow_System SHALL 设置欠流故障码
5. THE PowerWindow_System SHALL 将故障码存储至 NvM_Manager

### 需求 6: 电压监测与欠压保护

**用户故事:** 作为系统，我需要监测供电电压，以便在欠压情况下保护系统和电池。

#### 验收标准

1. THE PowerWindow_System SHALL 每 10ms 读取系统供电电压值
2. IF 电压低于欠压阈值标定参数持续超过 200ms，THEN THE PowerWindow_System SHALL 设置欠压故障码
3. WHEN 欠压故障被检测到，THE PowerWindow_System SHALL 禁止所有车窗移动操作
4. WHEN 电压恢复至正常范围超过 500ms，THE PowerWindow_System SHALL 清除欠压故障码并恢复正常功能

### 需求 7: 学习功能

**用户故事:** 作为系统，我需要学习车窗的上下限位置，以便建立准确的位置参考基准。

#### 验收标准

1. WHEN 系统首次上电且未完成学习，THE PowerWindow_System SHALL 设置未学习状态标志
2. WHEN 用户触发学习程序，THE PowerWindow_System SHALL 驱动车窗至完全关闭位置并记录上限值
3. WHEN 上限学习完成，THE PowerWindow_System SHALL 驱动车窗至完全打开位置并记录下限值
4. WHEN 上下限学习均完成，THE PowerWindow_System SHALL 设置学习完成标志并存储至 NvM_Manager
5. IF 学习未完成，THEN THE PowerWindow_System SHALL 禁止自动升降和防夹功能
6. THE PowerWindow_System SHALL 允许手动模式在未学习状态下工作

### 需求 8: 非易失性存储管理

**用户故事:** 作为系统，我需要将关键数据保存到非易失性存储器，以便在断电后恢复系统状态。

#### 验收标准

1. WHEN 系统初始化时，THE NvM_Manager SHALL 从非易失性存储器读取上次保存的车窗位置
2. WHEN 系统初始化时，THE NvM_Manager SHALL 从非易失性存储器读取学习状态和学习数据
3. WHEN 系统初始化时，THE NvM_Manager SHALL 从非易失性存储器读取最近的故障码列表
4. WHEN 车窗位置变化超过 5% 或系统关闭时，THE NvM_Manager SHALL 保存当前位置至非易失性存储器
5. WHEN 学习完成时，THE NvM_Manager SHALL 保存学习状态和上下限值至非易失性存储器
6. WHEN 新故障码产生时，THE NvM_Manager SHALL 保存故障码至非易失性存储器
7. IF 非易失性存储器读取失败，THEN THE NvM_Manager SHALL 使用默认值并设置 NVM 故障标志

### 需求 9: 诊断接口

**用户故事:** 作为诊断工具，我需要读取系统状态和故障信息，以便进行故障诊断和维护。

#### 验收标准

1. THE Diag_Interface SHALL 提供读取当前车窗位置百分比的接口
2. THE Diag_Interface SHALL 提供读取当前运行状态的接口
3. THE Diag_Interface SHALL 提供读取所有活动故障码的接口
4. THE Diag_Interface SHALL 提供读取历史故障码的接口
5. THE Diag_Interface SHALL 提供清除故障码的接口
6. THE Diag_Interface SHALL 提供读取学习状态的接口
7. THE Diag_Interface SHALL 提供触发学习程序的接口
8. THE Diag_Interface SHALL 提供读取所有标定参数当前值的接口

### 需求 10: 标定参数管理

**用户故事:** 作为标定工程师，我需要配置系统参数，以便针对不同车型优化系统性能。

#### 验收标准

1. THE PowerWindow_System SHALL 支持配置防夹电流阈值标定参数（范围: 5A - 20A）
2. THE PowerWindow_System SHALL 支持配置防夹回退距离标定参数（范围: 50mm - 200mm）
3. THE PowerWindow_System SHALL 支持配置位置上限标定参数（范围: 0 - 4095）
4. THE PowerWindow_System SHALL 支持配置位置下限标定参数（范围: 0 - 4095）
5. THE PowerWindow_System SHALL 支持配置欠压阈值标定参数（范围: 9V - 11V）
6. THE PowerWindow_System SHALL 支持配置过流阈值标定参数（范围: 15A - 30A）
7. THE PowerWindow_System SHALL 支持配置自动模式触发时间标定参数（范围: 300ms - 1000ms）
8. THE PowerWindow_System SHALL 在标定参数超出有效范围时使用默认值并记录配置错误

### 需求 11: 状态机管理

**用户故事:** 作为系统，我需要清晰的状态管理机制，以便确保车窗控制逻辑的正确性和可维护性。

#### 验收标准

1. THE State_Machine SHALL 支持以下状态: 未初始化、空闲、手动上升、手动下降、自动上升、自动下降、防夹回退、故障
2. THE State_Machine SHALL 在系统初始化时进入未初始化状态
3. WHEN 初始化完成且无故障，THE State_Machine SHALL 转换至空闲状态
4. THE State_Machine SHALL 仅允许在空闲状态下接受新的升降命令
5. THE State_Machine SHALL 在检测到故障时立即转换至故障状态
6. WHILE 处于故障状态，THE State_Machine SHALL 拒绝所有移动命令直至故障被清除
7. THE State_Machine SHALL 记录所有状态转换事件用于诊断

### 需求 12: 周期性主函数

**用户故事:** 作为 AUTOSAR 运行时环境，我需要周期性调用系统主函数，以便驱动整个控制逻辑。

#### 验收标准

1. THE PowerWindow_System SHALL 提供 PowerWindow_MainFunction_10ms 作为 10ms 周期 Runnable
2. WHEN PowerWindow_MainFunction_10ms 被调用，THE PowerWindow_System SHALL 依次执行输入读取、状态机更新、安全监控、输出控制、诊断更新
3. THE PowerWindow_MainFunction_10ms SHALL 在 10ms 时间窗口内完成所有处理
4. THE PowerWindow_System SHALL 维护执行时间统计用于性能监控

### 需求 13: 模块化架构

**用户故事:** 作为开发者，我需要清晰的模块化架构，以便于代码维护和测试。

#### 验收标准

1. THE PowerWindow_System SHALL 将功能划分为以下模块: PowerWindow、StateMachine、Safety、NvM、Diag、Interface
2. THE PowerWindow_System SHALL 为每个模块提供独立的头文件和实现文件
3. THE PowerWindow_System SHALL 通过明确定义的接口实现模块间通信
4. THE PowerWindow_System SHALL 确保模块间无循环依赖
5. THE PowerWindow_System SHALL 为每个模块提供初始化和去初始化函数

### 需求 14: 测试支持

**用户故事:** 作为测试工程师，我需要完整的测试用例和 mock 接口，以便验证系统功能。

#### 验收标准

1. THE PowerWindow_System SHALL 提供 mock 接口用于模拟硬件输入输出
2. THE PowerWindow_System SHALL 提供测试用例覆盖手动升降场景
3. THE PowerWindow_System SHALL 提供测试用例覆盖自动升降场景
4. THE PowerWindow_System SHALL 提供测试用例覆盖防夹触发和回退场景
5. THE PowerWindow_System SHALL 提供测试用例覆盖欠压故障场景
6. THE PowerWindow_System SHALL 提供测试用例覆盖位置传感器异常场景
7. THE PowerWindow_System SHALL 提供测试用例覆盖 NVM 保存和恢复场景
8. THE PowerWindow_System SHALL 确保所有测试用例可编译和执行

### 需求 15: 文档完整性

**用户故事:** 作为项目成员，我需要完整的文档，以便理解系统设计和使用方法。

#### 验收标准

1. THE PowerWindow_System SHALL 提供 README.md 包含项目概述和快速开始指南
2. THE PowerWindow_System SHALL 提供软件需求文档描述所有功能需求
3. THE PowerWindow_System SHALL 提供架构设计文档描述模块划分和接口定义
4. THE PowerWindow_System SHALL 提供状态机设计文档包含状态转换图和转换条件
5. THE PowerWindow_System SHALL 提供接口说明文档描述所有公开 API
6. THE PowerWindow_System SHALL 提供使用说明文档描述编译、配置和运行步骤
7. THE PowerWindow_System SHALL 提供测试说明文档描述测试环境搭建和执行方法
