import axios from 'axios';

const API_BASE = 'http://localhost:5182';

export const getGames = async () => {
  const response = await axios.get(`${API_BASE}/api/Games`);
  return response.data;
};

export const getGame = async (id) => {
  const response = await axios.get(`${API_BASE}/api/Games/${id}`);
  return response.data;
};

export const createGame = async (game) => {
  const response = await axios.post(`${API_BASE}/api/Games`, game);
  return response.data;
};

export const updateGame = async (id, game) => {
  const response = await axios.put(`${API_BASE}/api/Games/${id}`, game);
  return response.data;
};

export const deleteGame = async (id) => {
  const response = await axios.delete(`${API_BASE}/api/Games/${id}`);
  return response.data;
}; 