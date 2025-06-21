# Game Store API - Clean Architecture with Controller-Based API

A RESTful API for managing a video game store catalog, built with ASP.NET Core following clean architecture principles and using traditional controller-based approach with Swagger documentation.

## Overview

This API provides endpoints to manage a collection of video games, allowing operations such as creating, reading, updating, and deleting game entries. Each game entry includes details like name, genre, price, and release date.

## Architecture

The application follows Clean Architecture principles with the following layers:

### 1. **Presentation Layer** (Controllers)
- **Location**: `Controllers/`
- **Responsibility**: Handle HTTP requests/responses, input validation, and routing using traditional controller-based approach
- **Files**: 
  - `GamesController.cs` - Game-related API endpoints using `[ApiController]` and `[Route("api/[controller]")]`
  - `GenresController.cs` - Genre-related API endpoints using `[ApiController]` and `[Route("api/[controller]")]`

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

### 3. **Controller Pattern**
- Traditional ASP.NET Core MVC controllers
- Uses `[ApiController]` attribute for automatic model validation
- Implements RESTful conventions with proper HTTP status codes

### 4. **DTO Pattern (Data Transfer Objects)**
- Separates API contracts from internal data models
- Provides clear boundaries between layers
- Located in the `Dtos/` directory

### 5. **Dependency Injection**
- All dependencies are injected through constructors
- Services and repositories are registered in `Program.cs`
- Promotes loose coupling and testability

## API Endpoints

### Games (`/api/Games`)
- `GET /api/Games` - List all games
- `GET /api/Games/{id}` - Get game by ID
- `POST /api/Games` - Create new game
- `PUT /api/Games/{id}` - Update existing game
- `DELETE /api/Games/{id}` - Delete game

### Genres (`/api/Genres`)
- `GET /api/Genres` - List all genres
- `GET /api/Genres/{id}` - Get genre by ID

## Swagger Documentation

The API includes comprehensive Swagger/OpenAPI documentation:

- **Swagger UI**: Automatically opens at `http://localhost:5182` when running in development
- **Interactive Testing**: Test all endpoints directly from the Swagger UI
- **Auto-Generated Documentation**: Based on XML comments in controllers
- **Request/Response Schemas**: Complete documentation of all DTOs

### Accessing Swagger
1. Run the application: `dotnet run`
2. Browser automatically opens to Swagger UI
3. Or manually navigate to: `http://localhost:5182`

## Error Handling

The application uses custom exceptions for better error handling:

- `GameStoreException` - Base exception for all application errors
- `GameNotFoundException` - Thrown when a game is not found
- `GenreNotFoundException` - Thrown when a genre is not found
- `ValidationException` - Thrown for validation errors

All controllers implement comprehensive try-catch blocks with proper HTTP status codes:
- `200 OK` - Successful operations
- `201 Created` - Resource created successfully
- `204 No Content` - Successful operations with no content
- `400 Bad Request` - Validation errors or invalid input
- `404 Not Found` - Resource not found
- `500 Internal Server Error` - Server errors

## Technologies Used

- **.NET 9**: Modern, cross-platform framework
- **ASP.NET Core**: Web framework for building HTTP-based services
- **Entity Framework Core**: Object-database mapper for .NET
- **SQLite**: Lightweight, serverless database engine
- **Swagger/OpenAPI**: API documentation and testing
- **Controller-Based API**: Traditional ASP.NET Core MVC approach

## Getting Started

1. Ensure you have .NET 9 SDK installed
2. Clone the repository
3. Navigate to the project directory
4. Run the following commands:
   ```bash
   dotnet restore
   dotnet run
   ```
5. The API will be available at `http://localhost:5182`
6. Swagger UI will automatically open in your browser

## Database

The application uses SQLite as its database, with Entity Framework Core handling the database operations. The database file (`GameStore.db`) is automatically created and migrated on first run.

## Development

- Built with Visual Studio 2022
- Uses modern C# features
- Implements proper HTTP status codes and REST conventions
- Includes parameter validation
- Supports database migrations
- Follows clean architecture principles
- Includes comprehensive logging throughout the application

## Recent Updates

### Controller-Based API Conversion
- ✅ **Converted from Minimal API to Controller-based API**
- ✅ **Added `GamesController` and `GenresController`**
- ✅ **Implemented `[ApiController]` and `[Route("api/[controller]")]` attributes**
- ✅ **Added comprehensive XML documentation for Swagger**

### Swagger Integration
- ✅ **Added Swagger/OpenAPI documentation**
- ✅ **Configured automatic browser opening**
- ✅ **Added interactive API testing interface**
- ✅ **Generated comprehensive API documentation**

### Enhanced Error Handling
- ✅ **Implemented custom exception types**
- ✅ **Added comprehensive try-catch blocks in controllers**
- ✅ **Proper HTTP status code responses**
- ✅ **Detailed error logging**

### Clean Architecture Implementation
- ✅ **Repository Pattern** for data access abstraction
- ✅ **Service Layer Pattern** for business logic
- ✅ **Dependency Injection** for all components
- ✅ **DTO Pattern** for API contracts
- ✅ **Separation of concerns** across all layers

## Benefits of the Current Architecture

1. **Separation of Concerns**: Each layer has a specific responsibility
2. **Testability**: Services and repositories can be easily unit tested
3. **Maintainability**: Changes in one layer don't affect others
4. **Scalability**: Easy to add new features or modify existing ones
5. **Dependency Inversion**: High-level modules don't depend on low-level modules
6. **API Documentation**: Comprehensive Swagger documentation for easy testing
7. **Error Handling**: Robust error handling with proper HTTP status codes
8. **Logging**: Comprehensive logging throughout the application 