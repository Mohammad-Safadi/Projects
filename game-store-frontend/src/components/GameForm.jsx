import { useState, useEffect } from 'react';

export default function GameForm({ initialData = null, onSubmit, onCancel, genres }) {
  const formatDateForInput = (dateString) => {
    if (!dateString) return '';
    const date = new Date(dateString);
    return date.toISOString().split('T')[0];
  };

  const [form, setForm] = useState({
    name: initialData?.name || '',
    genreId: initialData?.genreId || (genres[0]?.id || 1),
    price: initialData?.price || '',
    releaseDate: formatDateForInput(initialData?.releaseDate) || '',
  });
  const [error, setError] = useState('');

  useEffect(() => {
    if (initialData) {
      setForm({
        name: initialData.name || '',
        genreId: initialData.genreId || (genres[0]?.id || 1),
        price: initialData.price || '',
        releaseDate: formatDateForInput(initialData.releaseDate) || '',
      });
    } else {
      // Reset form when adding new game
      setForm({
        name: '',
        genreId: genres[0]?.id || 1,
        price: '',
        releaseDate: '',
      });
    }
  }, [initialData, genres]);

  const handleChange = (e) => {
    const { name, value } = e.target;
    setForm((f) => ({ ...f, [name]: value }));
  };

  const handleSubmit = (e) => {
    e.preventDefault();
    if (!form.name || !form.price || !form.releaseDate) {
      setError('All fields are required.');
      return;
    }
    setError('');
    onSubmit({
      name: form.name,
      genreId: Number(form.genreId),
      price: Number(form.price),
      releaseDate: form.releaseDate,
    });
  };

  return (
    <form onSubmit={handleSubmit} className="bg-white p-6 rounded shadow w-full max-w-md mx-auto">
      <h2 className="text-xl font-bold mb-4">{initialData?.id ? 'Edit Game' : 'Add Game'}</h2>
      {error && <div className="bg-red-100 text-red-700 px-3 py-2 rounded mb-2">{error}</div>}
      <div className="mb-4">
        <label className="block mb-1 font-medium">Name</label>
        <input
          type="text"
          name="name"
          value={form.name}
          onChange={handleChange}
          className="w-full border px-3 py-2 rounded"
          required
        />
      </div>
      <div className="mb-4">
        <label className="block mb-1 font-medium">Genre</label>
        <select
          name="genreId"
          value={form.genreId}
          onChange={handleChange}
          className="w-full border px-3 py-2 rounded"
        >
          {genres.map((g) => (
            <option key={g.id} value={g.id}>{g.name}</option>
          ))}
        </select>
      </div>
      <div className="mb-4">
        <label className="block mb-1 font-medium">Price</label>
        <input
          type="number"
          name="price"
          value={form.price}
          onChange={handleChange}
          className="w-full border px-3 py-2 rounded"
          min="1"
          max="100"
          step="0.01"
          required
        />
      </div>
      <div className="mb-4">
        <label className="block mb-1 font-medium">Release Date</label>
        <input
          type="date"
          name="releaseDate"
          value={form.releaseDate}
          onChange={handleChange}
          className="w-full border px-3 py-2 rounded"
          required
        />
      </div>
      <div className="flex justify-end space-x-2">
        <button type="button" onClick={onCancel} className="bg-gray-300 hover:bg-gray-400 text-gray-800 px-4 py-2 rounded">Cancel</button>
        <button type="submit" className="bg-blue-600 hover:bg-blue-700 text-white px-4 py-2 rounded">Save</button>
      </div>
    </form>
  );
} 