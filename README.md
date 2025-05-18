# Project Name

[![Qt](https://img.shields.io/badge/Qt-6.x-green)](https://www.qt.io/)
[![C++](https://img.shields.io/badge/C++-17-blue)](https://isocpp.org/)
[![CMake](https://img.shields.io/badge/CMake-3.5+-brightgreen)](https://cmake.org/)

## Overview

C++17/Qt6 application featuring modern OOP principles and design patterns for robust architecture.

## Tech Stack

### Core Technologies
- **Language**: C++17 (OOP, STL, RAII)
- **Framework**: Qt 6 (GUI, Signals/Slots, Widgets)
- **Build System**: CMake

### Implemented Design Patterns
| Category       | Patterns                                                                 |
|----------------|--------------------------------------------------------------------------|
| **Creational** | Singleton, Factory Method, Abstract Factory, Builder, Prototype         |
| **Structural** | Adapter, Decorator, Proxy, Facade                                       |
| **Behavioral** | Strategy, Command, Observer (Qt signals/slots)                          |

## 💡 Key Features

### Architectural Highlights
- **Modular Architecture** - Decoupled game logic (Model) from Qt-based UI (View) via Observer pattern
- **SOLID Compliance** - Clear separation of interfaces (`IUnit`, `ILogger`) and implementations
- **Undo/Redo System** - Command Pattern implementation with `QUndoStack`
- **Dynamic Buff System** - Decorator Pattern for runtime unit modifications
- **Theme Management** - Abstract Factory for Light/Dark modes
- **Resource Optimization** - Lazy Initialization for GUI components

### Prerequisites
- Qt 6.x
- CMake 3.5+
- C++17 compatible compiler
