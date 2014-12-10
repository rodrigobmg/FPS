/*
	Collision manager:
	- rozdeli entity podla octree
	- spravy test kolizie kazdy objekt ( teda len vo svojom octree ) s kazdym a vytvo list
	- tento list sa pouziva raycasting, visibility aj vo fyzike
	- najprv sa tstuje rychle AA poda toho sa priradi do Octree
	- Octree sa porovnava referencia
	public bool AABox(); // porovnava priemery, priorita 1
	public bool Circle(); // porovnava priemery, priorita 1
	public bool BBBox(); // z uhlom, priorita 2
	public bool SubParticles(; // ak je to lietadlo tak subcasti porovna, casto krat nepouzijeme priorita 3.
	public virtual bool SubParticles() { return true; }

*/

