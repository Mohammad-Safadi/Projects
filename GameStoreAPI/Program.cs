
using GameStoreAPI.Data;
using GameStoreAPI.Endpoints;

var builder = WebApplication.CreateBuilder(args);

var connectionString = builder.Configuration.GetConnectionString("GameStoreContext") ??
    throw new InvalidOperationException("Connection string 'GameStoreContext' not found.");

builder.Services.AddSqlite<GameStoreContext>(connectionString);

var app = builder.Build(); 

app.MapEndpoints();

app.MapGenreEndpoints();

await app.MigrateDbAsync();

app.Run();
 