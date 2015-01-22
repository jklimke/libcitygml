#include <osgViewer/Viewer>
#include <osgDB/Registry>
#include <osgDB/ReadFile>
#include <osg/Node>

#include <osgGA/SphericalManipulator>
#include <osgGA/StateSetManipulator>
#include <osgViewer/ViewerEventHandlers>

#include <iostream>

int main(int argc, char *argv[])
{
    osg::setNotifyLevel(osg::INFO);

    std::cout << "Using plugin directory: " << PLUGIN_BIN_DIR << std::endl;
    osgDB::Registry::instance()->getLibraryFilePathList().push_front(PLUGIN_BIN_DIR);

    if (argc < 2) {
        std::cerr << "No citygml file specified..." << std::endl;
        return 1;
    }

    osg::ref_ptr<osg::Node> node = osgDB::readNodeFile(argv[1]);

    if (node == nullptr) {
        std::cerr << "Failed to load file " << argv[1] << std::endl;
        return 1;
    }

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
    viewer.run();

    return 0;
}
