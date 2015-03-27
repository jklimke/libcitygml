#include <osgViewer/Viewer>
#include <osgDB/Registry>
#include <osgDB/ReadFile>
#include <osg/Node>

#include <osgGA/SphericalManipulator>
#include <osgGA/StateSetManipulator>
#include <osgGA/GUIEventHandler>
#include <osgGA/GUIEventAdapter>
#include <osgGA/GUIActionAdapter>
#include <osgViewer/ViewerEventHandlers>

#include <osgUtil/LineSegmentIntersector>
#include <osgUtil/Optimizer>

#include <iostream>

class PickingHandler : public osgGA::GUIEventHandler {
public:

    PickingHandler() {
        m_xMouseCoordAtLastPress = -1;
        m_yMouseCoordAtLastPress = -1;
    }

    // EventHandler interface
    virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osg::Object*, osg::NodeVisitor*) {
        if (ea.getEventType() == osgGA::GUIEventAdapter::RELEASE && ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON) {

            if (m_xMouseCoordAtLastPress != ea.getX() || m_yMouseCoordAtLastPress != ea.getY()) {
                return false;
            }

            return printDescriptionOfIntersectedDrawable(ea, aa);


        } else if (ea.getEventType() == osgGA::GUIEventAdapter::PUSH && ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON) {
            m_xMouseCoordAtLastPress = ea.getX();
            m_yMouseCoordAtLastPress = ea.getY();
        }

        return false;
    }

private:

    bool printDescriptionOfIntersectedDrawable(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa) {

        osgUtil::LineSegmentIntersector::Intersections intersections;
        if (!aa.computeIntersections(ea, intersections)) {
            return false;
        }


        osgUtil::LineSegmentIntersector::Intersection firstIntersection = *intersections.begin();

        if (firstIntersection.drawable == nullptr) {
            return false;
        }

        if (firstIntersection.drawable->getNumDescriptions() == 0) {
            std::cout << "Intersected drawable has no description:" << std::endl;
            return true;
        }

        std::cout << "Description of intersected drawable:" << std::endl;
        for (const std::string& desc : firstIntersection.drawable->getDescriptions()) {
            std::cout << "  " << desc << std::endl;
        }
        std::cout << std::endl;

        return true;
    }

    int m_xMouseCoordAtLastPress;
    int m_yMouseCoordAtLastPress;
};


int main(int argc, char *argv[])
{
    osg::setNotifyLevel(osg::WARN);

    std::cout << "Using plugin directory: " << PLUGIN_BIN_DIR << std::endl;
    osgDB::Registry::instance()->getLibraryFilePathList().push_front(PLUGIN_BIN_DIR);

    osgDB::Registry::instance()->addFileExtensionAlias("gml", "citygml");

    if (argc < 2) {
        std::cerr << "No citygml file specified..." << std::endl;
        return 1;
    }

    osg::ref_ptr<osgDB::Options> options = new osgDB::Options("usemaxlodonly storegeomids");

    std::cout << "Loading file: " << argv[1] << std::endl;
    osg::ref_ptr<osg::Node> node = osgDB::readNodeFile(argv[1], options);

    if (node == nullptr) {
        std::cerr << "Failed to load file " << argv[1] << std::endl;
        return 1;
    }

    std::cout << "Optimizing node..." << std::endl;
    osgUtil::Optimizer optimizer;
    optimizer.optimize(node, osgUtil::Optimizer::ALL_OPTIMIZATIONS);

    std::cout << "Setup viewer..." << std::endl;


    osg::ref_ptr<osgGA::SphericalManipulator> manip = new osgGA::SphericalManipulator();
    manip->setAutoComputeHomePosition(true);
    manip->setNode(node);

    osgViewer::Viewer viewer;
    viewer.setUpViewInWindow(0, 0, 800, 600);
    viewer.setSceneData(node);
    viewer.setCameraManipulator(manip);
    viewer.addEventHandler(new osgGA::StateSetManipulator(node->getOrCreateStateSet()));
    viewer.addEventHandler(new osgViewer::StatsHandler());
    viewer.addEventHandler(new osgViewer::WindowSizeHandler());
    viewer.addEventHandler(new PickingHandler());
    viewer.addEventHandler(new osgViewer::HelpHandler());
    viewer.run();

    return 0;
}

