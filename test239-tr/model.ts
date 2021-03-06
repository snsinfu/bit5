export interface User {
  id: string;
  name: string;
  tokenKey: string;
  tokenSecret: string;
}

export interface Board {
  id: string;
  name: string;
  user: string;
}

export interface Card {
  id: string;
  board: string;
  updateTime: number;
}

export interface Reaper {
  id: string;
  board: string;
  webhook: string;
  timeToLive: number;
}

export interface Session {
  id: string;
  user: string;
}
