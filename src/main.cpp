#include <Geode/modify/AppDelegate.hpp>

using namespace geode::prelude;

bool enabled = false;
bool closeAllFLAlertLayers = false;

class $modify(MyAppDelegate, AppDelegate) {
	static PauseLayer* getPauseLayer() {
		const auto scene = CCScene::get();
		if (scene) return scene->getChildByType<PauseLayer>(0);
		return nullptr;
	}
	static void unpauseOnUnfocusResumeGame() {
		if (!enabled) return;
		CCScene* scene = CCScene::get();
		if (!scene || scene->getChildrenCount() < 1) return;
		PauseLayer* pauseLayer = getPauseLayer();
		if (!pauseLayer) return;
		if (closeAllFLAlertLayers && scene->getChildren()) {
			for (const auto fooBar : CCArrayExt<CCObject*>(scene->getChildren())) {
				if (const auto isAlert = typeinfo_cast<FLAlertLayer*>(fooBar)) isAlert->removeMeAndCleanup();
			}
		}
		return pauseLayer->onResume(nullptr);
	}
	void applicationWillResignActive() {
		if (getPauseLayer()) return AppDelegate::applicationWillResignActive();
		AppDelegate::applicationWillResignActive();
		unpauseOnUnfocusResumeGame();
	}
	void applicationDidEnterBackground() {
		if (getPauseLayer()) return AppDelegate::applicationWillResignActive();
		AppDelegate::applicationDidEnterBackground();
		unpauseOnUnfocusResumeGame();
	}
};

$on_mod(Loaded) {
	enabled = Mod::get()->getSettingValue<bool>("enabled");
	closeAllFLAlertLayers = Mod::get()->getSettingValue<bool>("closeAllFLAlertLayers");
	listenForSettingChanges<bool>("enabled", [](bool newEnabled) {
		enabled = newEnabled;
	});
	listenForSettingChanges<bool>("closeAllFLAlertLayers", [](bool newCloseAllFLAlertLayers) {
		closeAllFLAlertLayers = newCloseAllFLAlertLayers;
	});
}