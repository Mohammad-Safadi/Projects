export default function GameTable({ games, onEdit, onDelete }) {
  const formatDate = (dateString) => {
    if (!dateString) return '';
    return new Date(dateString).toLocaleDateString();
  };

  return (
    <div className="overflow-x-auto">
      <table className="min-w-full bg-white rounded shadow">
        <thead>
          <tr>
            <th className="px-4 py-2">ID</th>
            <th className="px-4 py-2">Name</th>
            <th className="px-4 py-2">Genre</th>
            <th className="px-4 py-2">Price</th>
            <th className="px-4 py-2">Release Date</th>
            <th className="px-4 py-2">Actions</th>
          </tr>
        </thead>
        <tbody>
          {games.map((game) => (
            <tr key={game.id} className="border-t">
              <td className="px-4 py-2">{game.id}</td>
              <td className="px-4 py-2">{game.name}</td>
              <td className="px-4 py-2">{game.genre}</td>
              <td className="px-4 py-2">${game.price.toFixed(2)}</td>
              <td className="px-4 py-2">{formatDate(game.releaseDate)}</td>
              <td className="px-4 py-2 space-x-2">
                <button
                  className="bg-yellow-400 hover:bg-yellow-500 text-white px-3 py-1 rounded"
                  onClick={() => onEdit(game)}
                >
                  Edit
                </button>
                <button
                  className="bg-red-500 hover:bg-red-600 text-white px-3 py-1 rounded"
                  onClick={() => onDelete(game)}
                >
                  Delete
                </button>
              </td>
            </tr>
          ))}
        </tbody>
      </table>
    </div>
  );
} 