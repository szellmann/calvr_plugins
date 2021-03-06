#include "texturemanager.h"
#include <math.h>
#include <stdlib.h>

TextureManager::TextureManager(unsigned int gid) : m_gid(gid), _scene(0), m_isStereo(false)
{

}

TextureManager::~TextureManager()
{
	printf("Deleting a texture manager\n");
}

void TextureManager::SetSceneObject(cvr::SceneObject* scene)
{
	_scene = scene;
}

cvr::SceneObject* TextureManager::GetSceneObject()
{
	return _scene;
}

unsigned int TextureManager::GetVideoGID()
{
	return m_gid;
}
void TextureManager::AddGID(unsigned int gid)
{
	m_gidList.push_back(gid);
}

osg::Geode* TextureManager::AddTexture(unsigned int gid, std::map<unsigned int, GLuint> texmap, unsigned int width, unsigned int height)
{
	double xstart, ystart, zstart;
	double nrows, ncols, myrow, mycol;

	if (gid & 0x80000000)
	{
		nrows = ((gid & 0x3E000000) >> 25) + 1.;
        	ncols = ((gid & 0x00F80000) >> 19) + 1.;
		myrow = (gid & 0x0007E000) >> 13;
		mycol = (gid & 0x00001F80) >> 7;
	}
	else
	{
		nrows = 1.;
		ncols = 1.;
		myrow = 0.;
		mycol = 0.;
	}

	xstart = mycol * width - width/2.;
	ystart = myrow * height - height/2.;
	zstart = 2244.; // distance in mm for a 2048 pixel video stream with a 12.5mm lens
	TextureObject* to = new TextureObject(texmap);
	osg::Geometry* picture_quad = osg::createTexturedQuadGeometry(osg::Vec3(xstart, zstart, ystart), osg::Vec3(width, 0., 0.), osg::Vec3(0., 0., height));
	osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
	osg::Geode* geode = new osg::Geode;
	texture->setResizeNonPowerOfTwoHint(false);
	texture->setNumMipmapLevels(0);

	texture->setSubloadCallback(to);

	texture->setName("Video");
	texture->setDataVariance(osg::Object::STATIC);
	texture->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR);
	texture->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);

	picture_quad->getOrCreateStateSet()->setTextureAttributeAndModes(0, texture.get(), osg::StateAttribute::ON);
	picture_quad->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

	geode->addDrawable(picture_quad);


	return geode;

}

unsigned int TextureManager::GetVideoCount() const
{
	return m_gidList.size();
}

unsigned int TextureManager::GetVideoID(unsigned int idx) const
{
	if (idx < m_gidList.size())
		return m_gidList[idx];
	else
		return -1;
}

void TextureManager::SetStereo(STEREO_EYE eye)
{
	m_isStereo = true;
	m_eye = eye;
}

bool TextureManager::IsStereo()
{
	return m_isStereo;
}

STEREO_EYE TextureManager::GetStereo()
{
	return m_eye;
}
