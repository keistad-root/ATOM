#include "ExperimentAnalysis.hpp"

class ExeControlPanel {
private:
	ArgumentParser mParser;
	CppConfigFile mConfig;
public:
	ExeControlPanel(int argc, char** argv);
	void setConfig();
	const CppConfigFile& getConfig() const;
};

ExeControlPanel::ExeControlPanel(int argc, char** argv) : mParser(argc, argv) {
	mParser.setDescription("Draw plots for analysis data");
	mParser.add_argument("config").help("Config File").set_default("default").add_finish();
	mParser.parse_args();
}

void ExeControlPanel::setConfig() {
	std::string configPath = mParser.get_value<std::string>("config");
	mConfig = CppConfigFile(configPath);
}

const CppConfigFile& ExeControlPanel::getConfig() const {
	return mConfig;
}

int main(int argc, char** argv) {
	ExeControlPanel controller(argc, argv);
	controller.setConfig();

	TAnalyser* mAnalyser = new TAnalyser(controller.getConfig());
	mAnalyser->originMasking();
	mAnalyser->storeEvents();
	mAnalyser->setExpSettingLegend();
	mAnalyser->getHitmaps();
	mAnalyser->clusterize();
	mAnalyser->getClusterinfos();


	// controller.openInputFile();
	// controller.setExpDataSet();
	// controller.doBasicAnalysis();
	// controller.drawHitmap();
	// controller.clusterization();
	// controller.drawClustermapAndClustersize();
	// controller.doDivideBySize();
	// controller.drawClusterShapeInfos();

	// clusterAnalyser.saveHitmapByClustersize(*config->getConfig("Clustermap"));


	delete mAnalyser;
	mAnalyser = nullptr;

	return 0;
}