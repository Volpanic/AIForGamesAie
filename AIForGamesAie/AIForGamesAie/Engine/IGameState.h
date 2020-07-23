#pragma once


class IGameState
{
public:
	IGameState() { };
	virtual ~IGameState() { };

	virtual void Load() { };
	virtual void Unload() { };

	virtual void Update(float deltaTime) { };
	virtual void Draw() { };

protected:
private:
};