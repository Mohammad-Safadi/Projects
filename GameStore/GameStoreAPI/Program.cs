using GameStoreAPI.Data;
using GameStoreAPI.Repositories;
using GameStoreAPI.Services;

var builder = WebApplication.CreateBuilder(args);

// Configure URLs explicitly
builder.WebHost.UseUrls("http://localhost:5182", "https://localhost:7017");

var connectionString = builder.Configuration.GetConnectionString("GameStoreContext") ??
    throw new InvalidOperationException("Connection string 'GameStoreContext' not found.");

builder.Services.AddSqlite<GameStoreContext>(connectionString);

// Register repositories
builder.Services.AddScoped<IGameRepository, GameRepository>();
builder.Services.AddScoped<IGenreRepository, GenreRepository>();

// Register services
builder.Services.AddScoped<IGameService, GameService>();
builder.Services.AddScoped<IGenreService, GenreService>();

// Add controllers
builder.Services.AddControllers();

// Add Swagger/OpenAPI support
builder.Services.AddEndpointsApiExplorer();
builder.Services.AddSwaggerGen(c =>
{
    c.SwaggerDoc("v1", new Microsoft.OpenApi.Models.OpenApiInfo
    {
        Title = "Game Store API",
        Version = "v1",
        Description = "A RESTful API for managing a video game store catalog"
    });
});

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

// Configure the HTTP request pipeline
if (app.Environment.IsDevelopment())
{
    app.UseSwagger();
    app.UseSwaggerUI(c =>
    {
        c.SwaggerEndpoint("/swagger/v1/swagger.json", "Game Store API V1");
        c.RoutePrefix = string.Empty; // Serve Swagger UI at the app's root
    });
}

// Use CORS
app.UseCors();

// Use routing and map controllers
app.UseRouting();
app.MapControllers();

await app.MigrateDbAsync();

// Display startup information
Console.WriteLine("\n🚀 Game Store API is starting up!");
Console.WriteLine("📖 Swagger UI will be available at: http://localhost:5182");
Console.WriteLine("🔗 API Base URL: http://localhost:5182/api");
Console.WriteLine("📖 Opening Swagger UI in your default browser...\n");


// Open browser automatically in development mode
if (app.Environment.IsDevelopment())
{
    try
    {
        var processInfo = new System.Diagnostics.ProcessStartInfo
        {
            FileName = "http://localhost:5182",
            UseShellExecute = true
        };
        System.Diagnostics.Process.Start(processInfo);
        Console.WriteLine("✅ Browser opened successfully!");
    }
    catch (Exception ex)
    {
        // Fallback if automatic opening fails
        Console.WriteLine($"⚠️  Could not open browser automatically: {ex.Message}");
        Console.WriteLine("📖 Please open your browser and navigate to: http://localhost:5182");
    }
}

await app.RunAsync();



