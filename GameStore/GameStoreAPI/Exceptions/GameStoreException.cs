namespace GameStoreAPI.Exceptions;

public class GameStoreException : Exception
{
    public GameStoreException(string message) : base(message)
    {
    }

    public GameStoreException(string message, Exception innerException) : base(message, innerException)
    {
    }
}

public class GameNotFoundException : GameStoreException
{
    public GameNotFoundException(int id) : base($"Game with ID {id} was not found.")
    {
    }
}

public class GenreNotFoundException : GameStoreException
{
    public GenreNotFoundException(int id) : base($"Genre with ID {id} was not found.")
    {
    }
}

public class ValidationException : GameStoreException
{
    public ValidationException(string message) : base(message)
    {
    }
} 