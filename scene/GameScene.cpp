#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <random>

using namespace DirectX;

GameScene::GameScene() {}

GameScene::~GameScene() {

	
	delete model_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	textureHandle_ = TextureManager::Load("mario.jpg");
	
	//３Dモデル生成
	model_ = Model::Create();

	//親(0番)
	worldTransform_[PartId::Root].Initialize();
	//子(1番)
	worldTransform_[PartId::Spine].translation_ = {0, 0, 0}; // ローカル座標
	worldTransform_[PartId::Spine].parent_ = &worldTransform_[PartId::Root];
	worldTransform_[PartId::Spine].Initialize();
	// 1頭身
	worldTransform_[PartId::Chest].translation_ = {0, 0, 0}; // ローカル座標
	worldTransform_[PartId::Chest].parent_ = &worldTransform_[PartId::Spine];
	worldTransform_[PartId::Chest].Initialize();
	// 1頭身
	worldTransform_[PartId::ArmL].translation_ = {-2.5f, 0, 0}; // ローカル座標
	worldTransform_[PartId::ArmL].parent_ = &worldTransform_[PartId::Chest];
	worldTransform_[PartId::ArmL].Initialize();
	// 1頭身
	worldTransform_[PartId::Head].translation_ = {0, 2.5f, 0}; // ローカル座標
	worldTransform_[PartId::Head].parent_ = &worldTransform_[PartId::Chest];
	worldTransform_[PartId::Head].Initialize();
	// 1頭身
	worldTransform_[PartId::ArmR].translation_ = {2.5f, 0, 0}; // ローカル座標
	worldTransform_[PartId::ArmR].parent_ = &worldTransform_[PartId::Chest];
	worldTransform_[PartId::ArmR].Initialize();
	// 1頭身
	worldTransform_[PartId::Hip].translation_ = {0, -2.5f, 0}; // ローカル座標
	worldTransform_[PartId::Hip].parent_ = &worldTransform_[PartId::Spine];
	worldTransform_[PartId::Hip].Initialize();
	// 1頭身
	worldTransform_[PartId::LegL].translation_ = {-2.5f, -2.5f, 0}; // ローカル座標
	worldTransform_[PartId::LegL].parent_ = &worldTransform_[PartId::Hip];
	worldTransform_[PartId::LegL].Initialize();
	// 1頭身
	worldTransform_[PartId::LegR].translation_ = {2.5f, -2.5f, 0}; // ローカル座標
	worldTransform_[PartId::LegR].parent_ = &worldTransform_[PartId::Hip];
	worldTransform_[PartId::LegR].Initialize();

	//ビュープロダクションの初期化
	viewProjection_.Initialize();
}

void GameScene::Update() {
	// キャラクターの移動ベクトル
	XMFLOAT3 move = {0, 0, 0};

	// キャラクターの移動速度
	const float kCharacterSpeed = 0.1f;

	// 押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_LEFT)) {
		move = {-kCharacterSpeed, 0, 0};
	} else if (input_->PushKey(DIK_RIGHT)) {
		move = {kCharacterSpeed, 0, 0};
	}

	// 注視点移動(ベクトルの加算)
	worldTransform_[PartId::Root].translation_.x += move.x;
	worldTransform_[PartId::Root].translation_.y += move.y;
	worldTransform_[PartId::Root].translation_.z += move.z;

	// デバッグ用表示
	debugText_->SetPos(50, 150);
	debugText_->Printf(
	  "Root:(%f,%f,%f)", worldTransform_[PartId::Root].translation_.x,
	  worldTransform_[PartId::Root].rotation_.y, worldTransform_[PartId::Root].rotation_.z);

	worldTransform_[PartId::Root].UpdateMatrix();
	worldTransform_[PartId::Spine].UpdateMatrix();
	worldTransform_[PartId::Chest].UpdateMatrix();
	worldTransform_[PartId::Head].UpdateMatrix();
	worldTransform_[PartId::ArmL].UpdateMatrix();
	worldTransform_[PartId::ArmR].UpdateMatrix();
	worldTransform_[PartId::Hip].UpdateMatrix();
	worldTransform_[PartId::LegL].UpdateMatrix();
	worldTransform_[PartId::LegR].UpdateMatrix();
	// 1半身の回転
	const float kChestRotSpeed = 0.05f;
	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_U)) {
		worldTransform_[PartId::Chest].rotation_.y -= kChestRotSpeed;
	} else if (input_->PushKey(DIK_K)) {
		worldTransform_[PartId::Hip].rotation_.y += kChestRotSpeed;
	}
	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_I)) {
		worldTransform_[PartId::Chest].rotation_.y += kChestRotSpeed;
	} else if (input_->PushKey(DIK_J)) {
		worldTransform_[PartId::Hip].rotation_.y -= kChestRotSpeed;
	}
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
	/*model_->Draw(worldTransform_[PartId::Root], viewProjection_, textureHandle_);
	model_->Draw(worldTransform_[PartId::Spine], viewProjection_, textureHandle_);*/
	model_->Draw(worldTransform_[PartId::Chest], viewProjection_, textureHandle_);
	model_->Draw(worldTransform_[PartId::Head], viewProjection_, textureHandle_);
	model_->Draw(worldTransform_[PartId::ArmL], viewProjection_, textureHandle_);
	model_->Draw(worldTransform_[PartId::ArmR], viewProjection_, textureHandle_);
	model_->Draw(worldTransform_[PartId::Hip], viewProjection_, textureHandle_);
	model_->Draw(worldTransform_[PartId::LegL], viewProjection_, textureHandle_);
	model_->Draw(worldTransform_[PartId::LegR], viewProjection_, textureHandle_);

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
