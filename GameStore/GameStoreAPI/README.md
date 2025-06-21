# Game Store API - Clean Architecture Implementation

A RESTful API for managing a video game store catalog, built with ASP.NET Core following clean architecture principles.

## Overview

This API provides endpoints to manage a collection of video games, allowing operations such as creating, reading, updating, and deleting game entries. Each game entry includes details like name, genre, price, and release date.

## Architecture

The application follows Clean Architecture principles with the following layers:

### 1. **Presentation Layer** (Endpoints)
- **Location**: `Endpoints/`
- **Responsibility**: Handle HTTP requests/responses, input validation, and routing
- **Files**: 
  - `GamesEndpoints.cs` - Game-related API endpoints
  - `GenresEndpoints.cs` - Genre-related API endpoints

### 2. **Application Layer** (Services)
- **Location**: `Services/`
- **Responsibility**: Business logic, orchestration, and domain operations
- **Files**:
  - `IGameService.cs` - Game service interface
  - `GameService.cs` - Game service implementation
  - `IGenreService.cs` - Genre service interface
  - `GenreService.cs` - Genre service implementation

### 3. **Infrastructure Layer** (Repositories)
- **Location**: `Repositories/`
- **Responsibility**: Data access and persistence
- **Files**:
  - `IGameRepository.cs` - Game repository interface
  - `GameRepository.cs` - Game repository implementation
  - `IGenreRepository.cs` - Genre repository interface
  - `GenreRepository.cs` - Genre repository implementation

### 4. **Domain Layer** (Entities & DTOs)
- **Location**: `Entities/` and `Dtos/`
- **Responsibility**: Core business models and data transfer objects
- **Files**:
  - `Entities/Game.cs` - Game entity
  - `Entities/Genre.cs` - Genre entity
  - `Dtos/` - Various DTOs for API contracts

### 5. **Cross-Cutting Concerns**
- **Location**: `Exceptions/` and `Mapping/`
- **Responsibility**: Error handling and object mapping
- **Files**:
  - `Exceptions/GameStoreException.cs` - Custom exception types
  - `Mapping/` - Object mapping extensions

## Key Design Patterns

### 1. **Repository Pattern**
- Abstracts data access layer
- Separates business logic from data access logic
- Provides a clean interface for data operations

### 2. **Service Layer Pattern**
- Encapsulates business logic
- Handles transaction management
- Provides a clean API for the presentation layer

### 3. **DTO Pattern (Data Transfer Objects)**
- Separates API contracts from internal data models
- Provides clear boundaries between layers
- Located in the `Dtos/` directory

### 4. **Dependency Injection**
- All dependencies are injected through constructors
- Services and repositories are registered in `Program.cs`
- Promotes loose coupling and testability

## API Endpoints

### Games
- `GET /games` - List all games
- `GET /games/{id}` - Get game by ID
- `POST /games` - Create new game
- `PUT /games/{id}` - Update existing game
- `DELETE /games/{id}` - Delete game

### Genres
- `GET /genres` - List all genres
- `GET /genres/{id}` - Get genre by ID

## Error Handling

The application uses custom exceptions for better error handling:

- `GameStoreException` - Base exception for all application errors
- `GameNotFoundException` - Thrown when a game is not found
- `GenreNotFoundException` - Thrown when a genre is not found
- `ValidationException` - Thrown for validation errors

## Technologies Used

- **.NET 9**: Modern, cross-platform framework
- **ASP.NET Core**: Web framework for building HTTP-based services
- **Entity Framework Core**: Object-database mapper for .NET
- **SQLite**: Lightweight, serverless database engine
- **Minimal APIs**: Modern approach to building APIs in .NET

## Getting Started

1. Ensure you have .NET 9 SDK installed
2. Clone the repository
3. Navigate to the project directory
4. Run the following commands:
   ```bash
   dotnet restore
   dotnet run
   ```
5. The API will be available at `https://localhost:5001` or `http://localhost:5000`

## Database

The application uses SQLite as its database, with Entity Framework Core handling the database operations. The database file (`GameStore.db`) is automatically created and migrated on first run.

## Development

- Built with Visual Studio 2022
- Uses modern C# features
- Implements proper HTTP status codes and REST conventions
- Includes parameter validation
- Supports database migrations
- Follows clean architecture principles

## Refactoring Summary

The codebase has been refactored to follow clean architecture best practices:

1. **Moved business logic** from endpoints to service classes
2. **Created service interfaces** for better testability and dependency injection
3. **Implemented repository pattern** for data access abstraction
4. **Added custom exceptions** for better error handling
5. **Configured dependency injection** for all services and repositories
6. **Used DTOs** in all API responses instead of entities
7. **Added comprehensive logging** throughout the application
8. **Implemented proper exception handling** with specific exception types

## Benefits of Clean Architecture

1. **Separation of Concerns**: Each layer has a specific responsibility
2. **Testability**: Services and repositories can be easily unit tested
3. **Maintainability**: Changes in one layer don't affect others
4. **Scalability**: Easy to add new features or modify existing ones
5. **Dependency Inversion**: High-level modules don't depend on low-level modules 