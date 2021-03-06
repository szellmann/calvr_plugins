#ifndef VVCLIENT_H
#define VVCLIENT_H

#include <cvrUtil/CVRSocket.h>

#include <osg/Camera>
#include <osg/Image>
#include <osg/Texture2D>

#include "SubImageInfo.h"

class VVClient
{
    public:
        VVClient(cvr::CVRSocket * socket);
        virtual ~VVClient();

        void preFrame();
        bool isError()
        {
            return _error;
        }

    protected:
        bool processSocket();
        bool processSubImage();

        void takeSubImage(SubImageInfo* info);
        void setSubImageParams(SubImageInfo * info, osg::Vec3 pos, float width, float height);

        bool _error;
        cvr::CVRSocket * _con;
        std::vector<SubImageInfo*> _imageInfoList;
};

#endif
