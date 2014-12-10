#include "class.BaseTerrain.h"
#include "../../Base/System/class.File.h"
using namespace std;
using namespace Structures;
using namespace Map;

BaseTerrain::BaseTerrain(char* filename) {
	m_filename = filename;
	m_heightmap = 0;

	LoadHeightMap();
	NormalizeHeightMap();
	CalculateNormals();
	CalculateTextureCoordinates();
	Initialize();
}
BaseTerrain::~BaseTerrain() {
	SAFE_DELETE(m_heightmap)
}
void BaseTerrain::LoadHeightMap()
{
	FILE* file;
	int error;
	UINT count;

	// Open the height map file in binary.
	error = fopen_s(&file, m_filename, "rb");
	if(error != 0) {
		throw new File::NotFound(m_filename);
	}

	// Read in the file header.
	BITMAPFILEHEADER bmp_header;
	BITMAPINFOHEADER bmp_info;
	count = fread(&bmp_header, sizeof(BITMAPFILEHEADER), 1, file);
	if(count != 1) {
		throw new File::Processing();
	}
	count = fread(&bmp_info, sizeof(BITMAPINFOHEADER), 1, file);
	if(count != 1) {
		throw new File::Processing();
	}

	// Save the dimensions of the terrain.
	m_terrainwidth = bmp_info.biWidth;
	m_terrainheight = bmp_info.biHeight;

	// Calculate the size of the bitmap image data.
	int imageSize = m_terrainwidth * m_terrainheight * 3;
	assert(imageSize);

	// Allocate memory for the bitmap image data.
	unsigned char* bitmapImage;
	bitmapImage = new unsigned char[imageSize];
	if(!bitmapImage) {
		throw new Memory::Exception();
	}

	// Move to the beginning of the bitmap data.
	fseek(file, bmp_header.bfOffBits, SEEK_SET);

	// Read in the bitmap image data.
	count = fread(bitmapImage, 1, imageSize, file);
	if(count != imageSize) {
		throw new File::Processing();
	}

	// Close the file.
	fclose(file);

	// Create the structure to hold the height map data.
	m_heightmap = new HeightMapType[m_terrainwidth * m_terrainheight];
	if(!m_heightmap) {
		throw new Memory::Exception();
	}

	// Read the image data into the height map.
	int i, j, k = 0, index;
	for(j = 0; j < m_terrainheight; j++) {
		for(i = 0; i < m_terrainwidth; i++) {
			index =  (m_terrainheight * j) + i;
			m_heightmap[ index ].position = vec( (float) i, (float) bitmapImage[k], (float) j );
			k += 3;
		}
	}
	SAFE_DELETE( bitmapImage )
}

void BaseTerrain::NormalizeHeightMap()
{
	assert(m_heightmap);

	int i, j;
	for(j = 0; j < m_terrainheight; j++) {
		for(i = 0; i < m_terrainwidth; i++) {
			m_heightmap[(m_terrainheight * j) + i].position.y /= 15.0f;
		}
	}
}

void BaseTerrain::CalculateNormals()
{
	assert(m_heightmap);
	
	// Create a temporary array to hold the un-normalized normal vectors.
	int i, j, index[4];
	vec* normals = new vec[(m_terrainheight-1) * (m_terrainwidth-1)];
	if(!normals) {
		throw new Memory::Exception();
	}

	// Go through all the faces in the mesh and calculate their normals.
	for(j=0; j < (m_terrainheight-1); j++)
	{
		for(i=0; i < (m_terrainwidth-1); i++)
		{
			// Corners
			index[0] = (j * (m_terrainheight-1)) + i;
			index[1] = (j * m_terrainheight) + i;
			index[2] = (j * m_terrainheight) + (i+1);
			index[3] = ((j+1) * m_terrainheight) + i;

			// Calculate the cross product of those two vectors to get the un-normalized value for this face normal.
			normals[ index[0] ].Cross( 
				(m_heightmap[ index[1] ].position - m_heightmap[ index[3] ].position ), 
				(m_heightmap[ index[3] ].position - m_heightmap[ index[2] ].position ) 
			);
		}
	}

	// Now go through all the vertices and take an average of each face normal 	
	// that the vertex touches to get the averaged normal for that vertex.
	int count, id;
	vec sum;

	for(j=0; j < m_terrainheight; j++)
	{
		for(i=0; i < m_terrainwidth; i++)
		{
			// Initialize the sum.
			sum.set(0.f);
			count = 0;

			// Bottom left face.
			if(((i-1) >= 0) && ((j-1) >= 0)) {
				id = ((j-1) * (m_terrainheight-1)) + (i-1);
				sum += normals[ id ];
				count++;
			}
			// Bottom right face.
			if((i < (m_terrainwidth-1)) && ((j-1) >= 0)) {
				id = ((j-1) * (m_terrainheight-1)) + i;
				sum += normals[ id ];
				count++;
			}
			// Upper left face.
			if(((i-1) >= 0) && (j < (m_terrainheight-1))) {
				id = (j * (m_terrainheight-1)) + (i-1);
				sum += normals[ id ];
				count++;
			}

			// Upper right face.
			if((i < (m_terrainwidth-1)) && (j < (m_terrainheight-1))) {
				id = (j * (m_terrainheight-1)) + i;
				sum += normals[ id ];
				count++;
			}

			// Normalize the final shared normal for this vertex and store it in the height map array.
			sum = sum / (float) count;
			sum.Normalize();
			m_heightmap[ (j * m_terrainheight) + i ].normal = sum;
		}
	}

	// Release the temporary normals.
	SAFE_DELETE(normals);
}
void BaseTerrain::CalculateTextureCoordinates()
{
	assert(m_heightmap);
	
	int incrementCount, i, j, tCount[2];
	float incrementValue;
	vec2 tCoordinate( 0.0f, 1.0f);

	incrementValue = (float) TEXTURE_REPEAT / (float) m_terrainwidth;	// Calculate how much to increment the texture coordinates by.
	incrementCount = m_terrainwidth / TEXTURE_REPEAT;					// Calculate how many times to repeat the texture.
	tCount[0] = tCount[1] = 0;

	// Loop through the entire height map and calculate the tu and tv texture coordinates for each vertex.
	for(j=0; j < m_terrainheight; j++)
	{
		for(i=0; i < m_terrainwidth; i++)
		{
			// Store the texture coordinate in the height map.
			m_heightmap[(m_terrainheight * j) + i].texture = tCoordinate;

			// Increment the tu texture coordinate by the increment value and increment the index by one.
			tCoordinate.x += incrementValue;
			tCount[0]++;

			// Check if at the far right end of the texture and if so then start at the beginning again.
			if(tCount[0] == incrementCount) {
				tCoordinate.x = 0.0f;
				tCount[0] = 0;
			}
		}

		// Increment the tv texture coordinate by the increment value and increment the index by one.
		tCoordinate.y -= incrementValue;
		tCount[1]++;

		// Check if at the top of the texture and if so then start at the bottom again.
		if(tCount[1] == incrementCount) {
			tCoordinate.y = 1.0f;
			tCount[1] = 0;
		}
	}
}

void BaseTerrain::Initialize()
{
	assert(m_heightmap);
	ULONG count = (m_terrainwidth - 1) * (m_terrainheight - 1) * 6;
	CreateVStructure(count);
	CreateIStructure(count);
	
	// Initialize the index to the vertex buffer.
	vec2 texture;
	int i, j, id, index[4];
	id = 0;

	// Load the vertex and index array with the terrain data.
	for(j=0; j < (m_terrainheight-1); j++)
	{
		for(i=0; i < (m_terrainwidth-1); i++)
		{
			index[0] = (m_terrainheight * j) + i;          // Bottom left.
			index[1] = (m_terrainheight * j) + (i+1);      // Bottom right.
			index[2] = (m_terrainheight * (j+1)) + i;      // Upper left.
			index[3] = (m_terrainheight * (j+1)) + (i+1);  // Upper right.

			// Upper left.
			// Modify the texture coordinates to cover the top edge.
			texture = m_heightmap[index[2]].texture;
			if(texture.y == 1.0f) { texture.y = 0.0f; }
			Vertex()[ id ].position = m_heightmap[ index[2] ].position;
			Vertex()[ id ].texture = texture;
			Vertex()[ id ].normal = m_heightmap[ index[2] ].normal;
			Index()[ id ] = id;
			id++;

			// Upper right.	
			// Modify the texture coordinates to cover the top and right edge.
			texture = m_heightmap[ index[3] ].texture;
			if(texture.x == 0.0f) { texture.x = 1.0f; }
			if(texture.y == 1.0f) { texture.y = 0.0f; }
			Vertex()[ id ].position = m_heightmap[ index[3] ].position;
			Vertex()[ id ].texture = texture;
			Vertex()[ id ].normal = m_heightmap[ index[3] ].normal;
			Index()[ id ] = id;
			id++;


			// Bottom left.
			Vertex()[ id ].position = m_heightmap[ index[0] ].position;
			Vertex()[ id ].texture = m_heightmap[ index[0] ].texture;
			Vertex()[ id ].normal = m_heightmap[ index[0] ].normal;
			Index()[ id ] = id;
			id++;


			// Bottom left.
			Vertex()[ id ].position = m_heightmap[ index[0] ].position;
			Vertex()[ id ].texture =m_heightmap[ index[0] ].texture;
			Vertex()[ id ].normal = m_heightmap[ index[0] ].normal;
			Index()[ id ] = id;
			id++;


			// Upper right.
			// Modify the texture coordinates to cover the top and right edge.
			texture = m_heightmap[index[3]].texture;
			if(texture.x == 0.0f) { texture.x = 1.0f; }
			if(texture.y == 1.0f) { texture.y = 0.0f; }
			Vertex()[ id ].position = m_heightmap[ index[3] ].position;
			Vertex()[ id ].texture = texture;
			Vertex()[ id ].normal = m_heightmap[ index[3] ].normal;
			Index()[ id ] = id;
			id++;


			// Bottom right.
			// Modify the texture coordinates to cover the right edge.
			texture = m_heightmap[index[1]].texture;
			if(texture.x == 0.0f) { texture.x = 1.0f; }
			Vertex()[ id ].position = m_heightmap[ index[1] ].position;
			Vertex()[ id ].texture = texture;
			Vertex()[ id ].normal = m_heightmap[ index[1] ].normal;
			Index()[ id ] = id;
			id++;
		}
	}

	CreateSVBuffer();
	CreateSIBuffer();
}
