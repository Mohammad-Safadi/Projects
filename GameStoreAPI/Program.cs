using GameStoreAPI.Data;
using GameStoreAPI.Endpoints;

var builder = WebApplication.CreateBuilder(args);

var connectionString = builder.Configuration.GetConnectionString("GameStoreContext") ??
    throw new InvalidOperationException("Connection string 'GameStoreContext' not found.");

builder.Services.AddSqlite<GameStoreContext>(connectionString);

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

var app = builder.Build(); 

// Use CORS
app.UseCors();

app.MapEndpoints();

app.MapGenreEndpoints();

await app.MigrateDbAsync();

app.Run();
 