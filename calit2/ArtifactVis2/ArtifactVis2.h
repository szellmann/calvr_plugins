#ifndef _ARTIFACTVIS2_
#define _ARTIFACTVIS2_

//#include <ConvertTools.h>
#include <PointCloudObject.h>
#include <ModelObject.h>
#include <LightObject.h>
//#include <OsgBulletTest.h>
#include <cvrKernel/CVRPlugin.h>
#include <cvrMenu/SubMenu.h>
#include <cvrMenu/MenuCheckbox.h>
#include <cvrMenu/MenuButton.h>
#include <cvrMenu/TabbedDialogPanel.h>
#include <cvrMenu/DialogPanel.h>
#include <cvrMenu/MenuRangeValue.h>
#include <cvrMenu/MenuTextButtonSet.h>
#include <cvrMenu/MenuList.h>
#include <cvrKernel/Navigation.h>
//#include <cvrInput/TrackingManager.h>
//#include <cvrKernel/InteractionManager.h>
#include <cvrKernel/SceneManager.h>
#include <cvrKernel/SceneObject.h>

#include <osg/AnimationPath>
#include <osg/PositionAttitudeTransform>
#include <osg/Material>
#include <osg/MatrixTransform>
#include <osgText/Text>
#include <osg/TextureCubeMap>
#include <osg/Texture2D>
#include <osg/LineWidth>
#include <osgShadow/ShadowedScene>
#include <osgShadow/SoftShadowMap>
#include <osgShadow/ShadowMap>
/*
//osgBullet
#include <osgbDynamics/RigidBody.h>
#include <osgbDynamics/MotionState.h>
#include <osgbDynamics/GroundPlane.h>
#include <osgbCollision/CollisionShapes.h>
#include <osgbCollision/RefBulletObject.h>
#include <osgbCollision/Utils.h>
#include <osgbDynamics/TripleBuffer.h>
#include <osgbDynamics/PhysicsThread.h>
#include <osgbInteraction/DragHandler.h>
#include <osgbInteraction/LaunchHandler.h>
#include <osgbInteraction/SaveRestoreHandler.h>

#include <osgwTools/Shapes.h>

#include <btBulletDynamicsCommon.h>
//..............................................
*/
#include <string>
#include <vector>
#include <map>

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#ifdef WIN32
//#include <ndir.h>
#include <direct.h>
#include <sys/types.h>
#include <windows.h>
#else
#include <sys/dir.h>
#include <sys/types.h>
#endif
//#include <X11/Xlib.h>



//#include <spnav.h>

//#include "skeleton.h"
//#include <shared/PubSub.h>
//#include <protocol/skeletonframe.pb.h>
//#include <protocol/colormap.pb.h>
//#include <protocol/depthmap.pb.h>
//#include <protocol/pointcloud.pb.h>
//#include <zmq.hpp>
//#include "DesignStateIntersector.h"
//#include "DesignObjectIntersector.h"

using namespace std;
using namespace osg;
using namespace cvr;
#define M_HEAD 1
#define M_LHAND 9
#define M_RHAND 15
#define M_LFOOT 20
#define M_RFOOT 24
#define CYLINDER 50
//cv::Mat depthRaw(480, 640, CV_16UC1);
const float DEPTH_SCALE_FACTOR = 255. / 4096.;
//std::map<int, Skeleton> mapIdSkel;

std::map< std::string, osg::ref_ptr<osg::Node> > objectMap;

//zmq::context_t context(1);
//SubSocket<RemoteKinect::SkeletonFrame>* skel_socket;

//uint32_t color_pixels[480 * 640];
//SubSocket<RemoteKinect::ColorMap>* color_socket;

//float depth_pixels[640 * 480];
//SubSocket<RemoteKinect::DepthMap>* depth_socket;

//SubSocket<RemoteKinect::PointCloud>* cloud_socket;
static osg::ref_ptr<osg::Geode> pointGeode;

// move somewhere else?
struct SelectableItem
{
    osg::ref_ptr<osg::MatrixTransform> scalet;
    osg::ref_ptr<osg::Geode> boxGeode;
    osg::ref_ptr<osg::MatrixTransform> mt;
    osg::ref_ptr<osg::MatrixTransform> rt;
    double scale;
    osg::Vec3 position;
    int lock;
    int type;
    SelectableItem()
    {
        mt = new osg::MatrixTransform();
        lock = -1;
        type = -1;
    }
    SelectableItem(osg::ref_ptr<osg::Geode> g, osg::ref_ptr<osg::MatrixTransform> _model, osg::ref_ptr<osg::MatrixTransform> m, osg::ref_ptr<osg::MatrixTransform> r, double _scale)
    {
        scalet = _model;
        boxGeode = g;
        mt = m;
        lock = -1;
        type = -1;
        rt = r;
        scale = _scale;
    }

    void setScale(double s)
    {
        scale = s;
        osg::Matrixd scalem;
        scalem.makeScale(s, s, s);
        scalet->setMatrix(scalem);
    }

    void lockTo(int lockedTo, int lockType)
    {
        lock = lockedTo;
        type = lockType;
        rt->removeChild(0, 1);
        rt->addChild(scalet);
    }

    void unlock()
    {
        lock = -1;
	type = -1;
        rt->removeChild(0, 1);
        //        rt->addChild(boxGeode); // change back to box when not manipulated -- with timeout? never?
        rt->addChild(scalet);
    }

};
int navLock =-1;
struct NavItem
{
    osg::ref_ptr<osg::MatrixTransform> scalet;
    osg::ref_ptr<osg::Geode> boxGeode;
    osg::ref_ptr<osg::MatrixTransform> mt;
    osg::ref_ptr<osg::MatrixTransform> rt;
    double scale;
    osg::Vec3 position;
    int lock;
    NavItem()
    {
        mt = new osg::MatrixTransform();
        lock = -1;
    }
    NavItem(osg::ref_ptr<osg::Geode> g, osg::ref_ptr<osg::MatrixTransform> _model, osg::ref_ptr<osg::MatrixTransform> m, osg::ref_ptr<osg::MatrixTransform> r, double _scale)
    {
        scalet = _model;
        boxGeode = g;
        mt = m;
        lock = -1;
        rt = r;
        scale = _scale;
    }

    void setScale(double s)
    {
        scale = s;
        osg::Matrixd scalem;
        scalem.makeScale(s, s, s);
        scalet->setMatrix(scalem);
    }

    void lockTo(int lockedTo)
    {
        lock = lockedTo;
        rt->removeChild(0, 1);
        rt->addChild(scalet);
    }

    void unlock()
    {
        lock = -1;
        rt->removeChild(0, 1);
        //        rt->addChild(boxGeode); // change back to box when not manipulated -- with timeout? never?
        rt->addChild(scalet);
    }

};

struct Annotation
{

	std::string desc;
        std::string name;
        std::string type; 
        float scale; 
        osg::Vec3 pos;
        osg::Quat rot;
        osg::Vec3 lStart;
        osg::Vec3 lEnd;
        osg::Geode* text_geode;
        osgText::Text* textNode;
        cvr::SceneObject * so;
        bool active;
        bool visible;
	bool fromFile;
        bool deleted;
        //std::map<cvr::SceneObject*,cvr::MenuButton*> saveMap;
        cvr::MenuButton* saveMap;
        cvr::MenuButton* deleteMap;
        cvr::MenuCheckbox* activeMap;
        cvr::MenuCheckbox* visibleMap;
        osg::Geometry * connector;  
        osg::Geode* connectorGeode;
        osg::Group* connectorNode;
        osg::Geometry * geo;  
};
struct QueryGraph
{
        std::string name;
        std::string parent; 
        float scale; 
        osg::Vec3 pos;
        osg::Quat rot;
        osg::Vec3 lStart;
        osg::Vec3 lEnd;
        osg::Geode* text_geode;
        osgText::Text* textNode;
        cvr::SceneObject * so;
        bool active;
        bool visible;
	bool fromFile;
        bool deleted;
        cvr::MenuButton* saveMap;
        cvr::MenuButton* deleteMap;
        cvr::MenuCheckbox* activeMap;
        cvr::MenuCheckbox* visibleMap;
        osg::Geometry * connector;  
        osg::Geode* connectorGeode;
        osg::Group* connectorNode;
        osg::Geometry * geo;  

};
struct ScreenSetup
{

              float width;
              float height;
              float h;
              float p;
              float r;
              osg::Vec3 offsetScreen;
};
struct LineGroup
{

        std::string name;
        std::string type; 
        float scale; 
        osg::Vec3 pos;
        osg::Vec3 scenePos;
        osg::Quat rot;
        std::vector<osg::Vec3> vertex;
        std::vector<osg::Geometry*> connector;
        std::vector<osg::Sphere*> cubeShape;
        std::vector<osg::Geode*> connectorGeode;
        std::vector<osg::Geode*> cubeGeode;
        std::vector<osg::Geode*> text_geode;
        std::vector<osg::Geode*> f_geode;
        std::vector<osg::Geode*> l_geode;
        std::vector<osgText::Text*> label;
        std::vector<float> distance;
        std::vector<int> selected;
        float distanceTotal;
        osg::Switch* switchNode;
        //float distance;
        float angle[3];
        cvr::SceneObject * so;
        bool active;
        bool visible;
        bool editing;
	bool fromFile;
        bool deleted;
        bool open;
        cvr::MenuButton* saveMap;
        cvr::MenuButton* deleteMap;
        cvr::MenuCheckbox* activeMap;
        cvr::MenuCheckbox* editingMap;
        cvr::MenuCheckbox* visibleMap;
};
struct ArtifactAnnoTrack
{
      bool active;
      int q;
      int art;

};
struct DirFile
{
  string filename;
  string filetype;
  string path;

};
struct SelectModel
{
	string name;
        float scale; 
        osg::Vec3 pos;
        osg::Quat rot;
        osg::Vec3 orig;
        cvr::SceneObject * so;
        bool active;
        bool visible;
        bool selected;
        bool lockPos;
        bool lockRot;
        bool lockScale;
        bool lockGraph;
    osg::ref_ptr<osg::PositionAttitudeTransform> patmt;
    osg::ref_ptr<osg::MatrixTransform> rt;
    osg::ref_ptr<osg::MatrixTransform> scalet;
    int lockedTo;
    int lockedType;
    osg::Vec3d kpos;
        cvr::MenuButton* saveMap;
        cvr::MenuButton* resetMap;
        cvr::MenuCheckbox* activeMap;
        cvr::MenuCheckbox* visibleMap;
        cvr::MenuCheckbox* pVisibleMap;
        cvr::MenuCheckbox* dcMap;
        cvr::MenuCheckbox* scanMap;
        cvr::MenuCheckbox* cubeMap;
        std::vector<cvr::MenuCheckbox*> photoMap;
	//Store Different Model Type Transforms
	string currentModelType;
        osg::Node* currentModelNode;
	osg::Switch* switchNode;
        string scanModel;
        float scanScale;
        osg::Vec3 scanPos;
        osg::Quat scanRot;
        string dcModel;
        float dcScale;
        int dcIndex;
        osg::Vec3 dcPos;
        osg::Quat dcRot;
        string cubeModel;
	float cubeScale;
        osg::Vec3 cubePos;
        osg::Quat cubeRot;
        string frameModel;
        float frameScale;
        osg::Vec3 framePos;
        osg::Quat frameRot;


};
struct Model
{
	string name;
	string filename;
        string fullpath;
        string filetype;
        string modelType;
        string group;
        float scale; 
        osg::Vec3 pos;
        osg::Quat rot;
        osg::Vec3 origPos;
        osg::Quat origRot;
        float origScale;
        cvr::SceneObject * so;
	PointCloudObject * pcObject;
	ModelObject * modelObject;
	bool loaded;
        bool active;
        bool visible;
        bool selected;
        bool lockPos;
        bool lockRot;
        bool lockScale;
        bool lockGraph;
    int lockedTo;
    int lockedType;
        cvr::MenuButton* saveMap;
        cvr::MenuButton* saveNewMap;
        cvr::MenuButton* resetMap;
        cvr::MenuCheckbox* activeMap;
        cvr::MenuCheckbox* visibleMap;
        cvr::MenuRangeValue* rxMap;
        cvr::MenuRangeValue* ryMap;
        cvr::MenuRangeValue* rzMap;
	//Store Different Model Type Transforms
        osg::Node* currentModelNode;
	osg::Switch* switchNode;


};
struct PointCloud
{
	string name;
	string filename;
        string fullpath;
        string filetype;
        string modelType;
        string group;
        float scale; 
        osg::Vec3 pos;
        osg::Quat rot;
        osg::Vec3 origPos;
        osg::Quat origRot;
        float origScale;
        cvr::SceneObject * so;
	bool loaded;
        bool active;
        bool visible;
        bool selected;
        bool lockPos;
        bool lockRot;
        bool lockScale;
        bool lockGraph;
    int lockedTo;
    int lockedType;
        cvr::MenuButton* saveMap;
        cvr::MenuButton* saveNewMap;
        cvr::MenuButton* resetMap;
        cvr::MenuCheckbox* activeMap;
        cvr::MenuCheckbox* visibleMap;
        cvr::MenuRangeValue* rxMap;
        cvr::MenuRangeValue* ryMap;
        cvr::MenuRangeValue* rzMap;
	//Store Different Model Type Transforms
        osg::Node* currentModelNode;
	osg::Switch* switchNode;


};

class Artifact
{
public:
    std::string dc;
    std::vector<std::string> fields;
    std::vector<std::string> values;
    double pos[3];
    osg::Drawable* drawable;
    osgText::Text* label;
    osg::Vec3 modelPos;
    osg::Vec3 modelOriginalPos;
    bool visible;
    bool selected;
    bool showLabel;
    double distToCam;
  
    osg::ref_ptr<osg::PositionAttitudeTransform> patmt;
    osg::ref_ptr<osg::MatrixTransform> rt;
    osg::ref_ptr<osg::MatrixTransform> scalet;
    int lockedTo;
    int lockedType;
    osg::Vec3d kpos;
    double scale;

    Artifact()
    {
        pos[0] = 0.0;
        pos[1] = 0.0;
        pos[2] = 0.0;
        showLabel = true;
    }

    void setScale(double s)
    {
        scale = s;
        osg::Matrixd scalem;
        scalem.makeScale(s, s, s);
        scalet->setMatrix(scalem);
    }
    //Detail Graph
    Annotation* annotation;
    //Select Model
    SelectModel* model;
};


class ArtifactVis2 : public cvr::MenuCallback, public cvr::CVRPlugin
{
public:
    ArtifactVis2();
    virtual ~ArtifactVis2();

    class compare
    {
    public:
        bool operator()(const Artifact* a1, const Artifact* a2)
        {
            return ((*a1).distToCam < (*a2).distToCam);
        }
    };
    void message(int type, char* data);

    bool init();
    void getDirFiles(const string& dirname, std::vector<DirFile* > & entries, std::string types);
    bool processEvent(cvr::InteractionEvent* event);
    bool statusSpnav;
    bool _handOn;
    void menuCallback(cvr::MenuItem* item);
    void preFrame();
    void turnOffAll();
    void setDCVisibleStatus(std::string dc, bool status);
    void updateVisibleStatus();
    std::string parseDate(std::string date);
    static ArtifactVis2* instance();
    std::vector<osg::Vec3> getArtifactsPos();
    float selectArtifactSelected();
    osg::Matrix getSelectMatrix();
    void setSelectMatrix(osg::Matrix& mat);
    bool _selectActive;
    bool _shiftActive;
    bool _rotActive;
    bool _grabActive;
    int _activeArtifact;
    int currentKey;
    void testSelected();
    std::string getCharacterAscii(int code);
    void readAnnotationFile();
    void readLineGroupFile();
    void saveBookmark(osg::Matrix headMat, float scale);
    bool nvidia;
    osg::Program* pgm1;
    float initialPointScale;// = ConfigManager::getFloat("Plugin.Points.PointScale", 0.001f);

     cvr::MenuCheckbox* _createCylinderCB;
     osg::Geode* createSelectSphere(osg::Vec3 currentPos);
     osg::Vec3 findBestSelectedPoint(osg::Matrix handMat,osg::Vec3Array* points);
     osg::Vec3Array* vecPoints;
     void createCylinder();
     std::vector<osg::Vec3> cylinderPoints;
     osg::ref_ptr<osg::Geode> first_geode; 
     osg::ref_ptr<osg::Geode> second_geode; 
     osg::ref_ptr<osg::Geode> third_geode; 


    std::vector<SelectableItem> selectableItems;
    void moveCam(double, double, double, double, double, double, double, double);
    void flyTo(int i);
    void createSelObj(osg::Vec3 pos, std::string, float radius);
/*
	osgbDynamics::TripleBuffer tBuf;
	osgbDynamics::MotionStateList msl;
	osg::ref_ptr< osg::Node > modelNode;
        osg::ref_ptr< osgbInteraction::SaveRestoreHandler > srh;
        btDiscreteDynamicsWorld* bulletWorld;
        osg::ref_ptr<osg::Group> bulletRoot;
*/
        double prevSimTime;
        bool physicsOn;
protected:

    void testPhysics();
    static ArtifactVis2* _artifactvis2;
    int _testA;
    bool ArtifactVis2On;
    bool secondInitComplete;
    struct Locus
    {
        std::vector<std::string> fields;
        std::vector<std::string> values;
        std::string id;
        std::string name;
        osg::Geometry* geom;
        osg::Geometry* tgeom;
        osg::ref_ptr<osg::Geode> fill_geode;
        osg::ref_ptr<osg::Geode> line_geode;
        osg::ref_ptr<osg::Geode > top_geode;
        osg::Geode* text_geode;
        bool visible;
        std::vector<osg::Vec3d> coordsTop;
        std::vector<osg::Vec3d> coordsBot;
        osgText::Text* label;
    };
    struct Table
    {
        std::string name;
        cvr::SubMenu* queryMenu;
        cvr::MenuButton* genQuery;
        cvr::MenuButton* clearConditions;
        cvr::MenuButton* saveQuery;
        cvr::SubMenu* viewQuery;
        cvr::MenuButton* addOR;
        cvr::MenuButton* removeOR;
        cvr::SubMenu* conditions;
        std::vector<cvr::SubMenu*> querySubMenu;
        std::vector<cvr::SubMenu*> querySubMenuSlider;
        std::vector<cvr::MenuTextButtonSet*> queryOptions;
        std::vector<cvr::MenuCheckbox*> querySlider;
        std::vector<std::vector<std::string> > sliderEntry;
        std::vector<cvr::MenuList*> queryOptionsSlider;
	std::vector<std::vector<std::string> > uniqueByColumn;
        std::vector<std::string> columns;
        cvr::MenuText* query_view;
        std::string current_query;
        std::vector<QueryGraph*> query_graph; 
    };
    struct FlyPlace
    {
        std::vector<std::string> name;
        std::vector<float> scale;
        std::vector<double> x;
        std::vector<double> y;
        std::vector<double> z;
        std::vector<double> rx;
        std::vector<double> ry;
        std::vector<double> rz;
        std::vector<double> rw;

    };

    struct QueryGroup
    {
        std::string name;
        std::string query;
        bool sf;
        std::vector<Artifact*> artifacts;
        std::vector<Locus*> loci;
        bool active;
        std::string timestamp;
        std::string kmlPath;
        osg::ref_ptr<osg::MatrixTransform> sphereRoot;
        bool updated;
        osg::Vec3d center;
    };
    //eventually struct Annotation
    osgText::Text* textNode;
    std::vector<Annotation*> _annotations;
    std::vector<ArtifactAnnoTrack*> _artifactAnnoTrack;
    std::vector<ArtifactAnnoTrack*> _artifactModelTrack;
    void deactivateAllAnno();
    void deactivateAllArtifactAnno();
    void deactivateAllArtifactModel();
    bool artifactPanelExists(int q, int art);
    bool artifactModelExists(int q, int art);
    void activateArtifactFromQuery(int q, int art);
    void activateModelFromQuery(int q, int art);
    void recreateConnector(int q, int art);
    int findActiveAnnot();
    void updateAnnoLine();
    void updateArtifactLine();
    void updateArtifactModel();
    void resetArtifactModelOrig(int q, int art);
    void switchModelType(string type,int q, int art);
    void deactivateModelSwitches(int q, int art);
    int convertDCtoIndex(string dc);
//    mxml_node_t  getTree(string filename);
    void saveAnnotationGraph();
    void saveLineGroup();
    void createArtifactPanel(int q, int art, string desc);
    void createArtifactModel(int q, int art, string desc);

    bool lineGroupsEditing;
    void startLineObject(int hand, int head); 
    int _editingHand;
    int _editingHead;
    int selectClosestVertice(int n);
    void updateClosedLine(int i,int index);
    void addToLineSelection(int i,int index);
    void removeFromLineSelection(int i,int index,int indexSelected);
    int vertLineSelected(int i,int index);
    void pullLineFace();
    void addLineVertex(int i,int index); 
    void closeLineVertex(int i); 
    void updateLineGroup(); 
    std::vector<LineGroup*> _lineGroups;
    std::vector<ArtifactAnnoTrack*> _lineGroupTrack;



    FlyPlace* _flyplace;
    osg::ref_ptr<osg::Node> _models[676];
    osg::ref_ptr<osg::Node> defaultDcModel;
    bool _modelLoaded[676];
    cvr::SubMenu* _modelDisplayMenu;
    cvr::MenuCheckbox* _turnOnArtifactVis2;
    std::vector<cvr::MenuButton*> _reloadModel;
    cvr::SubMenu* _pcDisplayMenu;
    std::vector<cvr::MenuButton*> _reloadPC;
    cvr::SubMenu* _hudDisplayMenu;
    std::vector<cvr::MenuButton*> _reloadHud;
    cvr::SubMenu* _avMenu;
    cvr::SubMenu* _displayMenu;
    cvr::SubMenu* _artifactDisplayMenu;
    std::vector<cvr::SubMenu*> _queryOptionMenu;
    std::vector<cvr::MenuCheckbox*> _queryOption;
    std::vector<int> _querySfIndex;
    std::vector<cvr::MenuCheckbox*> _queryOptionLoci;
    std::vector<int> _queryLociIndex;
    std::vector<cvr::MenuCheckbox*> _showPCCB;
    std::vector<cvr::MenuCheckbox*> _showHudCB;
    std::vector<cvr::MenuCheckbox*> _showModelCB;
    std::vector<cvr::MenuCheckbox*> _showPointCloudCB;
    std::vector<cvr::SubMenu*> _showQueryInfo;
    std::vector<cvr::SubMenu*> _showQueryInfoLoci;
    std::vector<cvr::MenuCheckbox*> _queryDynamicUpdate;
    std::vector<cvr::MenuText*> _queryInfo;
    std::vector<cvr::MenuButton*> _eraseQuery;
    std::vector<cvr::MenuButton*> _centerQuery;
    std::vector<cvr::MenuCheckbox*> _toggleLabel;  //new
    cvr::SubMenu* _locusDisplayMenu;
    cvr::MenuTextButtonSet* _locusDisplayMode;
    cvr::MenuCheckbox* _selectArtifactCB;
    cvr::MenuCheckbox* _manipArtifactCB;
    cvr::MenuCheckbox* _createAnnotations;
    cvr::MenuCheckbox* _createMarkup;
    //Kinect
    cvr::MenuCheckbox* _scaleBar;

    cvr::SubMenu* _flyMenu;
    std::vector<cvr::MenuButton*> _goto;
    cvr::MenuButton* _bookmarkLoc;
    //cvr::MenuButton * _goto; //new
    cvr::MenuCheckbox* _selectCB;
    cvr::SubMenu* _tablesMenu;
    osg::MatrixTransform* _root;

    cvr::TabbedDialogPanel* _artifactPanel;

    cvr::TabbedDialogPanel* _infoPanel;
    cvr::TabbedDialogPanel* _utilsPanel;
    cvr::TabbedDialogPanel* _qsPanel;
    cvr::TabbedDialogPanel* _bookmarkPanel;

     
    std::vector<DirFile*> entries;
    cvr::PopupMenu* _filePanel;
    cvr::MenuCheckbox* _fileMenu;
    std::vector<cvr::MenuButton*> fileButton;
    std::string _fileManagerType;
    cvr::MenuCheckbox* _clickFileManager;
    cvr::MenuCheckbox* _modelFileManager;
    cvr::MenuCheckbox* _pcFileManager;
    cvr::MenuCheckbox* _demFileManager;
    cvr::MenuCheckbox* _shpFileManager;
    cvr::MenuCheckbox* _artifactFileManager;
    cvr::MenuCheckbox* _locusFileManager;
    cvr::MenuButton* _resetFileManager;
    cvr::MenuButton* _upFileManager;
    cvr::MenuButton* _downFileManager;

    cvr::MenuCheckbox* _utilsMenu;
    cvr::MenuCheckbox* _qsMenu;
    cvr::MenuCheckbox* _bookmarksMenu;
    cvr::MenuButton* _artifactsDropDown;
    bool artifactsDropped;
    cvr::MenuButton* _lociDropDown;
    bool lociDropped;
    cvr::MenuButton* _modelDropDown;
    bool modelDropped;
    cvr::MenuButton* _pcDropDown;
    bool pcDropped;

    cvr::DialogPanel* _selectionStatsPanel;

    std::string _picFolder;

    std::vector<std::string> _dcList;
    std::map<std::string, bool> _dcVisibleMap;

    std::vector<Table*> _tables;
    std::vector<QueryGroup*> _query;

    osg::Vec3 _selectStart;
    osg::Vec3 _selectCurrent;

    osg::Material* _defaultMaterial;

    osg::Vec4 _colors[729];

    //osg::LOD * _my_own_root;
    osg::ref_ptr<osgShadow::ShadowedScene>  _shadowRoot;
    //osg::ref_ptr<osgShadow::SoftShadowMap>  sm;
    osg::ref_ptr<osgShadow::ShadowMap>  sm;
    LightObject* lightObject;
    //OsgBulletTest* bulletTest; 

    std::vector<osg::Vec3Array* > _coordsPC;
    std::vector<osg::Vec4Array* > _colorsPC;
    std::vector<osg::Vec3d> _avgOffset;
    std::vector<osg::Node* > _modelSFileNode;
    std::vector<osg::Node* > _hudFileNode;
    std::vector<osg::ref_ptr<osg::MatrixTransform> > _siteRoot;
    std::vector<osg::ref_ptr<osg::MatrixTransform> > _pcRoot;
    std::vector<osg::ref_ptr<osg::MatrixTransform> > _hudRoot;


    std::vector<Model* > _models3d;
    std::vector<Model* > _pointClouds;



    std::vector<osg::Vec3d> _sitePos;
    std::vector<osg::Vec3d> _siteScale;
    std::vector<osg::Vec3d> _siteRot;
    std::vector<osg::Vec3d> _pcPos;
    std::vector<osg::Vec3d> _pcScale;
    std::vector<osg::Vec3d> _pcRot;
    std::vector<osg::Vec3d> _hudPos;
    std::vector<osg::Vec3d> _hudScale;
    std::vector<osg::Vec3d> _hudRot;
    osg::Vec3 grabCurrentPos;
    osg::Vec3 _grabCurrentRot;
    osg::Vec3 rhit;
    bool _hudActive;
    std::vector<int> _pcFactor; //New
    osg::ref_ptr<osg::MatrixTransform> _selectBox;
    osg::ref_ptr<osg::MatrixTransform> _selectMark;
    osg::ref_ptr<osg::MatrixTransform> _selectModelLoad; //New Add

    osg::Node* _modelFileNode;
    osg::Node* _modelFileNode2;

    double _selectRotx;
    double _selectRoty;
    double _selectRotz;
    double _snum;
    osg::ref_ptr<osg::MatrixTransform> _scaleBarModel; //New Add
    osg::Vec3 _modelartPos; //New Add
    double _xRotMouse; //New Add
    double _yRotMouse; //New Add
    //float _LODmaxRange;
    float _sphereRadius;
    float _vertexRadius;


    bool _ossim;
    bool _osgearth;

    std::string getTimeModified(std::string file);
    int dc2Int(std::string dc);
    void loadScaleBar(osg::Vec3d start);  //new
    void setActiveArtifact(int _lockedTo, int _lockedType, int art, int query);
    void readQuery(QueryGroup* query);
    std::vector<std::string> getSelectedQueries();
    void listArtifacts();
    void displayArtifacts(QueryGroup* query);
    osg::Drawable* createObject(std::string dc, float tessellation, osg::Vec3d& pos);
    void clearConditions(Table* t);
    void readPointCloud(int index);
    void readSiteFile(int index);
    void readHudFile(int index);
    void reloadHudFile(int index);
    void readLocusFile(QueryGroup* query);

    osg::Matrix getHandToObjectMatrix();
    osg::Matrix getHandToObjectMatrix2(int hand, int head);
    osg::Matrix getHandToSceneMatrix();
    void parseModelXml(bool useHandPos);
    void parsePCXml(bool useHandPos, std::string filepath, std::string type);
    void saveModelConfig(Model* saveModel, bool newConfig);
    std::string manualEnterName; 
    void saveTo3Dkml(string name,string filename, string file, string filetype, Vec3 pos, Quat rot, float scaleFloat, string q_type, string q_group); 
    void addNewModel(int i);
    void addNewPCTest(int index);
    void addNewPC(int i);
    Vec3 matrix_to_euler(osg::Matrix colMatrix);
    float ClampUnity(float x);
    std::string newSelectedFile;
    std::string newSelectedName;
    std::string newSelectedType;
    bool newFileAvailable;
    void findAllModels();
//Menu System Functions

    void secondInit();
    void createShadowLighting();
    void menuSetup();
    void updateDropDowns();
    void addCheckBoxMenuItems(std::vector<cvr::MenuCheckbox*> checkBox);
    void removeCheckBoxMenuItems(std::vector<cvr::MenuCheckbox*> checkBox);
    void addSubMenuItems(std::vector<cvr::SubMenu*> menu);
    void removeSubMenuItems(std::vector<cvr::SubMenu*> menu);
    std::vector<cvr::SubMenu*> _modelMenus;
    std::vector<cvr::SubMenu*> _pcMenus;
    std::vector<std::string> _modelGroup;
    std::vector<std::string> _pcGroup;
    std::string getKmlArray(string file);
    void addToModelDisplayMenu(string group, cvr::MenuCheckbox* site);
    void addToPcDisplayMenu(string group, cvr::MenuCheckbox* site);
    std::string getPathFromFilePath(string filepath);
    std::string getFileFromFilePath(string filepath);

    void setupSiteMenu();
    void setupHudMenu();
    void reloadSite(int index);  //New
    void setupLocusMenu();
    void setupQuerySelectMenu();
    void setupTablesMenu();
    void setupQueryMenu(Table* table);
    void setupUtilsMenu();
    void setupFileMenu();
    void tempStackPhotos();
    void setupVisualQuery();
    void newQueryGraph(int tableIndex,std::string parent, std::string name, int order);
    void generateScreen();
    std::vector<ScreenSetup*>  readScreenConfig(std::string filename);
    void updateFileMenu(std::string dir, int scroll);
    int _currentScroll;
    std::string _currentDir;
    void initSelectBox();
    void updateSelect();
    std::string getCurrentQuery(Table* t);
    bool modelExists(const char* filename);
    void loadModels();
    void rotateModel(double rx, double ry, double rz);
    void setupFlyToMenu();
void loadAnnotationGraph(int inc);
void loadLineGroup(int i);
void createAnnotationFile(osg::Matrix tie);
void updateHudMovement(int i, cvr::TrackedButtonInteractionEvent * tie,float _moveDistance, osg::Vec3 _menuPoint);
    //Space Navigator
    float transMult, rotMult;
    float transcale, rotscale;
//void updateSceneObjectMovement(DSIntersector::DSIntersector* mDSIntersector, cvr::TrackedButtonInteractionEvent * tie);
	osg::Quat prevHandRot;
        std::vector<std::string> scanDirectory(const char *sDir);
        void newFileLoad(std::string filename, std::string type, bool useHandPos);
        void newLoadedFileSetup(std::string name, std::string filetype, std::string filepath, std::string modelType, osg::Vec3 pos);
        void updateLoadMenu(DirFile* entry);
        std::vector<DirFile*> getSubDirFiles(string dir, string filename, string types);
        void recursiveLoadMenu(std::vector<DirFile*> entries, string types);
        osgShadow::ShadowedScene* getShadowRoot();
//	btDiscreteDynamicsWorld* initPhysics();

//	osg::Transform* makeModel( const std::string& fileName, const int index, btDynamicsWorld* bw, osg::Vec3 pos, osgbInteraction::SaveRestoreHandler* srh );

//	osg::MatrixTransform* makeCow( btDynamicsWorld* bw, osg::Vec3 pos, osgbInteraction::SaveRestoreHandler* srh );
	
};

#endif
