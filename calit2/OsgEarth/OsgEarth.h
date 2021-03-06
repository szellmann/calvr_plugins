#ifndef _OSGEARTH_
#define _OSGEARTH_

#include <cvrKernel/CVRPlugin.h>
#include <cvrKernel/InteractionManager.h>
#include <cvrKernel/NodeMask.h>
#include <cvrMenu/SubMenu.h>
#include <cvrMenu/MenuButton.h>
#include <cvrMenu/MenuCheckbox.h>
#include <cvrMenu/MenuRangeValue.h>

#include <osgEarth/Map>
#include <osgEarth/MapNode>
#include <osgEarth/Utils>
#include <osgEarth/TerrainOptions>

#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>

#include <osgEarthDrivers/tms/TMSOptions>
#include <osgEarthDrivers/arcgis/ArcGISOptions>
#include <osgEarthDrivers/gdal/GDALOptions>

#include <osg/MatrixTransform>

#include <string>
#include <vector>

class OsgEarth : public cvr::CVRPlugin, public cvr::MenuCallback
{
    public:        
        OsgEarth();
        virtual ~OsgEarth();
        
	    bool init();
        int getPriority() { return 51; }
        void preFrame();
        bool processEvent(cvr::InteractionEvent * event);
        bool buttonEvent(int type, int button, int hand, const osg::Matrix & mat);
        bool mouseButtonEvent (int type, int button, int x, int y, const osg::Matrix &mat);
        void menuCallback(cvr::MenuItem * item);
        double getSpeed(double distance);
        void processNav(double speed);
        void processMouseNav(double speed);
        virtual void message(int type, char *&data, bool collaborative=false);



    protected:
        osgEarth::Map * _map;
        osgEarth::MapNode * _mapNode;

        cvr::SubMenu * _osgEarthMenu;
        cvr::MenuCheckbox * _navCB;
        cvr::MenuCheckbox * _visCB;
            
        bool _navActive;
        int _navHand;
        osg::Matrix _navHandMat;
                       
        bool _mouseNavActive;
        int _startX,_startY;
        int _currentX,_currentY;
        bool _movePointValid;
        osg::Vec3d _movePoint;

        //osg::ref_ptr<osgEarth::Map> _inactiveMap;
        cvr::SubMenu * _osgEarthLayers;
        cvr::SubMenu * _osgEarthShapes;

        //std::vector< std::pair<cvr::MenuCheckbox*, osgEarth::ImageLayer*> > _layers;
        //std::vector< std::pair<cvr::MenuCheckbox*, osgEarth::ModelLayer*> > _shapes;
        std::vector< std::pair<cvr::MenuRangeValue*, osgEarth::ImageLayer*> > _layers;
        std::vector< std::pair<cvr::MenuCheckbox*, osgEarth::ModelLayer*> > _shapes;


 	//bool _setting_viewpoint;
	//osgEarthUtil::Viewpoint _pending_viewpoint;
        //double _pending_viewpoint_duration_s;
        //bool _has_pending_viewpoint;

	//void setViewPoint( const Viewpoint& vp, double duration_s = 0.0);
	//void cancelViewpointTransition() { _setting_viewpoint = false; }
};

#endif
