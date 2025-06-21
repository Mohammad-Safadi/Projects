import { Link } from 'react-router-dom';

export default function Navbar() {
  return (
    <nav className="bg-blue-600 text-white px-4 py-3 mb-6">
      <div className="container mx-auto flex items-center justify-between">
        <span className="font-bold text-lg">Game Store</span>
        <div>
          <Link to="/" className="hover:underline">Games</Link>
        </div>
      </div>
    </nav>
  );
} 