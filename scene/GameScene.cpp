#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

using namespace DirectX;

//インクリメント
GameScene::GameScene() {}

//デストラクタ
GameScene::~GameScene() {

	delete model_;

}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("mario.jpg");

	//3Dモデル
	model_ = Model::Create();

	//X,Y,Z方向のスケーリング
	worldTransform_.scale_ = {5.0f, 5.0f, 5.0f};

	// X,Y,Z軸周りの回転角を設定
	worldTransform_.rotation_ = {XM_PI / 4.0f, XM_PI/4.0, 0.0f};

	// X,Y,Z軸周りの平行移動を設定
	worldTransform_.translation_ = {10.0f, 10.0f, 10.0f};

    //ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

}

void GameScene::Update() {

	debugText_->SetPos(10,10);
	debugText_->Printf("translation:(10.000000,10.000000,10.000000)");
	debugText_->SetPos(10, 30);
	debugText_->Printf("rotation:(0.785389,0.785389,0.000000)");
	debugText_->SetPos(10, 50);
	debugText_->Printf("scale:(5.000000,5.000000,5.000000)");
	
	
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	
	//3Dモデル描画
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
	 

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
