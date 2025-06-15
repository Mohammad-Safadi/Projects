# Game Store API

A RESTful API for managing a video game store catalog, built with ASP.NET Core.

## Overview

This API provides endpoints to manage a collection of video games, allowing operations such as creating, reading, updating, and deleting game entries. Each game entry includes details like name, genre, price, and release date.

## Technologies Used

- **.NET 7+**: Modern, cross-platform framework for building web applications
- **ASP.NET Core**: Web framework for building HTTP-based services
- **Entity Framework Core**: Object-database mapper for .NET
- **SQLite**: Lightweight, serverless database engine
- **RESTful API**: Architectural style for distributed hypermedia systems

## Design Patterns & Architecture

1. **Repository Pattern**
   - Abstracts data access layer
   - Separates business logic from data access logic

2. **DTO Pattern (Data Transfer Objects)**
   - Separates API contracts from internal data models
   - Provides clear boundaries between layers
   - Located in the `Dtos/` directory

3. **Clean Architecture**
   - Separation of concerns through layered architecture
   - Organized into distinct modules:
     - `Entities/`: Core business models
     - `Dtos/`: Data transfer objects
     - `Endpoints/`: API route definitions
     - `Data/`: Database context and configuration
     - `Mapping/`: Object mapping logic

4. **Minimal API Pattern**
   - Uses .NET's minimal API approach
   - Endpoint definitions in `GamesEndpoints.cs`
   - Route grouping for better organization

## API Endpoints

- `GET /games` - List all games
- `GET /games/{id}` - Get game by ID
- `POST /games` - Create new game
- `PUT /games/{id}` - Update existing game
- `DELETE /games/{id}` - Delete game

## Getting Started

1. Ensure you have .NET 7+ SDK installed
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