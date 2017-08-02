#ifndef MANAGER_H_INCLUDED
#define MANAGER_H_INCLUDED

class Manager {
public:
	virtual void init();
	virtual void update();
	virtual void render();
	virtual void cleanup();
};

#endif // MANAGER_H_INCLUDED
