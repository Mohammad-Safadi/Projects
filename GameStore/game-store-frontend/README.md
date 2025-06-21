# Game Store Frontend

A simple React frontend for the Game Store API.

## Features 
- List, add, edit, and delete  games 
- Connects to backend at http://localhost:5182
- Built with Vite, React, Tailwind CSS, and axios

## Setup

1. Install dependencies:
   ```bash
   npm install
   ```
2. Start the development server:
   ```bash
   npm run dev
   ```
3. The app will be available at http://localhost:5173

## Configuration
- The backend API base URL is set to `http://localhost:5182` in `src/api/api.js`.
- API endpoints use the `/api/Games` pattern for the controller-based API.
- Make sure your backend is running before using the frontend. 
