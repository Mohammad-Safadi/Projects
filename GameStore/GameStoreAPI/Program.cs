using GameStoreAPI.Data;
using GameStoreAPI.Endpoints;
using GameStoreAPI.Repositories;
using GameStoreAPI.Services;

var builder = WebApplication.CreateBuilder(args);

var connectionString = builder.Configuration.GetConnectionString("GameStoreContext") ??
    throw new InvalidOperationException("Connection string 'GameStoreContext' not found.");

builder.Services.AddSqlite<GameStoreContext>(connectionString);

// Register repositories
builder.Services.AddScoped<IGameRepository, GameRepository>();
builder.Services.AddScoped<IGenreRepository, GenreRepository>();

// Register services
builder.Services.AddScoped<IGameService, GameService>();
builder.Services.AddScoped<IGenreService, GenreService>();

// Add CORS
builder.Services.AddCors(options =>
{
    options.AddDefaultPolicy(policy =>
    {
        policy.AllowAnyOrigin()
              .AllowAnyMethod()
              .AllowAnyHeader();
    });
});

//just to test the CI
var app = builder.Build(); 

// Use CORS
app.UseCors();

app.MapEndpoints();

app.MapGenreEndpoints();

await app.MigrateDbAsync();

app.Run();
 