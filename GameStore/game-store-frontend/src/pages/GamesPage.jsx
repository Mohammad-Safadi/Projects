import { useEffect, useState } from 'react';
import { getGames, createGame, updateGame, deleteGame } from '../api/api';
import GameTable from '../components/GameTable';
import GameForm from '../components/GameForm';
import Loading from '../components/Loading';
import Error from '../components/Error';

// Static genres (should match backend seed data)
const GENRES = [
  { id: 1, name: 'Action' },
  { id: 2, name: 'Adventure' },
  { id: 3, name: 'Role-Playing' },
  { id: 4, name: 'Simulation' },
  { id: 5, name: 'Strategy' },
];

export default function GamesPage() {
  const [games, setGames] = useState([]);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState('');
  const [showForm, setShowForm] = useState(false);
  const [editingGame, setEditingGame] = useState(null);

  const fetchGames = async () => {
    setLoading(true);
    setError('');
    try {
      const data = await getGames();
      setGames(data);
    } catch (err) {
      setError('Failed to load games.');
    } finally {
      setLoading(false);
    }
  };

  useEffect(() => {
    fetchGames();
  }, []);

  const handleAdd = () => {
    setEditingGame(null);
    setShowForm(true);
  };

  const handleEdit = (game) => {
    setEditingGame(game);
    setShowForm(true);
  };

  const handleDelete = async (game) => {
    if (!window.confirm(`Delete game "${game.name}"?`)) return;
    setError('');
    try {
      await deleteGame(game.id);
      fetchGames();
    } catch {
      setError('Failed to delete game.');
    }
  };

  const handleFormSubmit = async (formData) => {
    setError('');
    try {
      if (editingGame) {
        await updateGame(editingGame.id, formData);
      } else {
        await createGame(formData);
      }
      
      setShowForm(false);
      await fetchGames(); // Refresh the games list
    } catch (err) {
      setError(`Failed to save game: ${err.response?.data?.message || err.message || 'Unknown error'}`);
    }
  };

  return (
    <div className="container mx-auto px-4">
      <div className="flex justify-between items-center mb-4">
        <h1 className="text-2xl font-bold">Games</h1>
        <button
          className="bg-blue-600 hover:bg-blue-700 text-white px-4 py-2 rounded"
          onClick={handleAdd}
        >
          Add Game
        </button>
      </div>
      {error && <Error message={error} />}
      {loading ? (
        <Loading />
      ) : (
        <GameTable games={games} onEdit={handleEdit} onDelete={handleDelete} />
      )}
      {showForm && (
        <div className="fixed inset-0 bg-black bg-opacity-40 flex items-center justify-center z-50">
          <div className="bg-white rounded shadow-lg p-6 w-full max-w-lg relative">
            <button
              className="absolute top-2 right-2 text-gray-500 hover:text-gray-700"
              onClick={() => setShowForm(false)}
            >
              &times;
            </button>
            <GameForm
              initialData={editingGame}
              onSubmit={handleFormSubmit}
              onCancel={() => setShowForm(false)}
              genres={GENRES}
            />
          </div>
        </div>
      )}
    </div>
  );
} 