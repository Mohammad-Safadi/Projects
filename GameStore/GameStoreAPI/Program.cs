using GameStoreAPI.Data;
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
//
// Open Swagger UI automatically in browser
if (app.Environment.IsDevelopment())
{
    var urls = app.Urls.ToArray();
    if (urls.Length > 0)
    {
        var url = urls[0];
        Console.WriteLine($"\n🚀 Swagger UI is available at: {url}");
        Console.WriteLine("📖 Opening Swagger UI in your default browser...\n");
        
        // Open browser automatically
        try
        {
            System.Diagnostics.Process.Start(new System.Diagnostics.ProcessStartInfo
            {
                FileName = url,
                UseShellExecute = true
            });
        }
        catch
        {
            // Fallback if automatic opening fails
            Console.WriteLine($"Please open your browser and navigate to: {url}");
        }
    }
}

app.Run();
 