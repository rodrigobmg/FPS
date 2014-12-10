
/*
	For more advanced behavior you want to make the AI remember 
	the last place where it saw the target. 
	That way, when LOS starts failing you have a position from which 
	the AI can start some kind of search behavior.
*/

/*
	You'll have to play with the numbers for your actual game but in practice 
	I find that AI doesn't really need to raytrace to it's targets more than 1-2 times a second. 

	Generally visibility is stored in a "VisibilityManager" or some other meta object 
	that does X number of raytraces per second and caches the results 
	for the AI to look up whenever it needs to.

	* Ray-Casting je asichronny
	- Teda sa poziada, posle pointer na funkciu ktora sa ma splnit a cele to bezi na novom vlakne
	- Po vykonani sa spusti dana funkcia
*/

/*
	Modern games can often incorporate other modeled senses like sound. 
	You have obejcts that make sound which is modeled by a radius. 
	Any AI within that radius will become aware of the sound maker. 
*/

/*
	Cesty pri stenach mozu byt preddefinovane a osoba hlada cestu, 
	spravi priamku a ked trafi priamku pred stenou tak ide po danej priamke...
*/