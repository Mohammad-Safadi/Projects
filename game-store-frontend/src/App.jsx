import { BrowserRouter as Router, Routes, Route } from 'react-router-dom';
import Navbar from './components/Navbar';
import GamesPage from './pages/GamesPage';

export default function App() {
  return (
    <Router>
      <Navbar />
      <div className="min-h-screen">
        <Routes>
          <Route path="/" element={<GamesPage />} />
        </Routes>
      </div>
    </Router>
  );
} 