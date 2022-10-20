#define WINDOW_WIDTH 1280//ウィンドウの幅
#define WINDOW_HEIGHT 768//ウィンドウの高さ

#include <Novice.h>

const char kWindowTitle[] = "後期チーム制作_原本";

typedef struct Vector2
{
	int x;
	int y;
};

//自機と敵の構造体
typedef struct ComData
{
	Vector2 pos;
	int rad;//半径
	int spd;
	int dead;//0:フラグオフ,1:フラグオン
};

//弾の構造体
typedef struct Bullet
{
	Vector2 pos;
	int rad;
	int spd;
	int bung;//0：発射されてない 1：発射された
};

//シーン切り替え用
enum Scene
{
	TITLE,
	PLAY,
	GAMEOVER,
	GAMECLEAR
};

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, WINDOW_WIDTH, WINDOW_HEIGHT);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	/*---自機---*/
	ComData player =
	{
		{720,704},
		32,
		0,
		0
	};

	/*---敵---*/
	ComData boss1 =
	{
		{240,96},
		32,
		0,
		0
	};
	int boss1Hp = 5000;

	/*---弾---*/
	//const int magazine = 5;
	Bullet bullet =
	{
		{-64,-64},
		16,
		0,
		0
	};
	//弾の最高速度
	const int maxSpd = 10;
	//弾の発射間隔
	int bulletTimer = 0;
	//跳ね返ししたかしてないか
	int reflect = 0;

	/*---システム関連---*/
	//シーン用
	Scene scene = TITLE;

	/*グラフィック関連*/
	int HPsprite[51] =
	{
			Novice::LoadTexture("./Resources./hp1.png"),
			Novice::LoadTexture("./Resources./hp2.png"),
			Novice::LoadTexture("./Resources./hp3.png"),
			Novice::LoadTexture("./Resources./hp4.png"),
			Novice::LoadTexture("./Resources./hp5.png"),
			Novice::LoadTexture("./Resources./hp6.png"),
			Novice::LoadTexture("./Resources./hp7.png"),
			Novice::LoadTexture("./Resources./hp8.png"),
			Novice::LoadTexture("./Resources./hp9.png"),
			Novice::LoadTexture("./Resources./hp10.png"),
			Novice::LoadTexture("./Resources./hp11.png"),
			Novice::LoadTexture("./Resources./hp12.png"),
			Novice::LoadTexture("./Resources./hp13.png"),
			Novice::LoadTexture("./Resources./hp14.png"),
			Novice::LoadTexture("./Resources./hp15.png"),
			Novice::LoadTexture("./Resources./hp16.png"),
			Novice::LoadTexture("./Resources./hp17.png"),
			Novice::LoadTexture("./Resources./hp18.png"),
			Novice::LoadTexture("./Resources./hp19.png"),
			Novice::LoadTexture("./Resources./hp20.png"),
			Novice::LoadTexture("./Resources./hp21.png"),
			Novice::LoadTexture("./Resources./hp22.png"),
			Novice::LoadTexture("./Resources./hp23.png"),
			Novice::LoadTexture("./Resources./hp24.png"),
			Novice::LoadTexture("./Resources./hp25.png"),
			Novice::LoadTexture("./Resources./hp26.png"),
			Novice::LoadTexture("./Resources./hp27.png"),
			Novice::LoadTexture("./Resources./hp28.png"),
			Novice::LoadTexture("./Resources./hp29.png"),
			Novice::LoadTexture("./Resources./hp30.png"),
			Novice::LoadTexture("./Resources./hp31.png"),
			Novice::LoadTexture("./Resources./hp32.png"),
			Novice::LoadTexture("./Resources./hp33.png"),
			Novice::LoadTexture("./Resources./hp34.png"),
			Novice::LoadTexture("./Resources./hp35.png"),
			Novice::LoadTexture("./Resources./hp36.png"),
			Novice::LoadTexture("./Resources./hp37.png"),
			Novice::LoadTexture("./Resources./hp38.png"),
			Novice::LoadTexture("./Resources./hp39.png"),
			Novice::LoadTexture("./Resources./hp40.png"),
			Novice::LoadTexture("./Resources./hp41.png"),
			Novice::LoadTexture("./Resources./hp42.png"),
			Novice::LoadTexture("./Resources./hp43.png"),
			Novice::LoadTexture("./Resources./hp44.png"),
			Novice::LoadTexture("./Resources./hp45.png"),
			Novice::LoadTexture("./Resources./hp46.png"),
			Novice::LoadTexture("./Resources./hp47.png"),
			Novice::LoadTexture("./Resources./hp48.png"),
			Novice::LoadTexture("./Resources./hp49.png"),
			Novice::LoadTexture("./Resources./hp50.png"),
	};

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0)
	{
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		/// ↓更新処理ここから

		switch (scene)
		{
			//タイトル画面
		case TITLE:

			//更新処理ここから

			if (keys[DIK_SPACE] && !preKeys[DIK_SPACE])
			{
				scene = PLAY;
			}

			//初期化するところ
			player.dead = 0;
			boss1.dead = 0;
			boss1Hp = 5000;
			bulletTimer = 0;			
			reflect = 0;

			//更新処理ここまで


			//描画処理ここから

			if (scene == TITLE)
			{
				Novice::ScreenPrintf(0, 0, "scene = %d", scene);
				Novice::ScreenPrintf(0, 20, "Hello");
			}

			//描画処理ここまで

			break;

			//プレイ画面
		case PLAY:

			//更新処理ここから
			bulletTimer++;
			//弾の装填
			if (bullet.bung == 0)
			{
				if (bulletTimer > 300)
				{
					bullet.pos.x = boss1.pos.x;
					bullet.pos.y = boss1.pos.y;
					bullet.bung = 1;
				}
			}
			//弾の発射
			if (bullet.bung == 1)
			{
				if (bulletTimer >= 300)
				{
					//反射する前
					if (reflect == 0)
					{
						bullet.spd++;
						//速度の限界値を決める
						if (bullet.spd > maxSpd)
						{
							bullet.spd = maxSpd;
						}
					}
					//反射した後
					if (reflect == 1)
					{
						bullet.spd--;
						//速度の限界値を決める
						if (bullet.spd < -maxSpd)
						{
							bullet.spd = -maxSpd;
						}
					}
				}
				//座標に加算
				bullet.pos.y += bullet.spd;
			}
			//弾を自機の画面に送る
			if (reflect == 0)
			{
				if (bullet.pos.y - bullet.rad >= WINDOW_HEIGHT)
				{
					if (bullet.pos.x >= 0 && bullet.pos.x < 480)
					{
						bullet.pos.x = 720;
						bullet.pos.y = 0;
					}
				}
			}
			//Space押して弾を反射(自機側の画面に弾が行ったときのみ反応)
			if (bullet.pos.x >= 480 && bullet.pos.x < 960)
			{
				if (keys[DIK_SPACE] && !preKeys[DIK_SPACE])
				{
					reflect = 1;//跳ね返しした
				}
			}
			//弾が自機に当たった時の処理
			if (bullet.pos.x >= 480 && bullet.pos.x < 960)
			{
				if (bullet.pos.y + bullet.rad > player.pos.y - player.rad)
				{
					bullet.pos.x = -64;
					bullet.pos.y = -64;
					bullet.spd = 1;
					player.dead = 1;
					reflect = 0;
					bullet.bung = 0;
					bulletTimer = 0;
				}
			}
			
			//弾を敵の画面に送る
			if (reflect == 1)
			{	
				if (bullet.pos.x >= 480 && bullet.pos.x < 960)
				{
					if (bullet.pos.y + bullet.rad <= 0)
					{
						bullet.pos.y = 768 + bullet.rad;
						bullet.pos.x = 240;
					}
				}
				//弾が敵に当たった時の処理
				if (bullet.pos.x >= 0 && bullet.pos.x < 480)
				{
					if (bullet.pos.y + bullet.rad < boss1.pos.y + boss1.rad)
					{
						bullet.pos.x = -64;
						bullet.pos.y = -64;
						bullet.spd = 1;
						reflect = 0;
						bullet.bung = 0;
						bulletTimer = 0;
						boss1Hp -= 500;
					}
				}
			}
			//ボスの死亡条件
			if (boss1Hp <= 0)
			{
				boss1.dead = 1;
			}

			//場所ごとの得点加算(記述予定)

			//ゲームオーバー条件
			if (player.dead == 1)
			{
				scene = GAMEOVER;
			}
			//ゲームクリア条件
			if (boss1.dead == 1)
			{
				scene = GAMECLEAR;
			}

			//更新処理ここまで


			//描画処理ここから

			//自機
			Novice::DrawBox(player.pos.x - player.rad, player.pos.y - player.rad, player.rad * 2, player.rad * 2, 0.0f, WHITE, kFillModeSolid);
			//弾
			if (bullet.bung == 1)
			{
				Novice::DrawEllipse(bullet.pos.x, bullet.pos.y, bullet.rad, bullet.rad, 0.0f, 0xFFFFFFFF, kFillModeSolid);
			}
			//敵
			Novice::DrawBox(boss1.pos.x - boss1.rad, boss1.pos.y - boss1.rad, boss1.rad * 2, boss1.rad * 2, 0.0f, BLACK, kFillModeSolid);
			//操作画面とUIの境界線
			Novice::DrawLine(960, 0, 960, WINDOW_HEIGHT, BLACK);
			//敵と自機の区切り
			Novice::DrawLine(480, 0, 480, WINDOW_HEIGHT, BLACK);
			//ボスのHPバー
			if (boss1Hp == 5000)
			{
				Novice::DrawSprite(10, 0, HPsprite[0], 1, 1, 0.0f, WHITE);
			}
			else if (boss1Hp >= 4900)
			{
				Novice::DrawSprite(10, 0, HPsprite[1], 1, 1, 0.0f, WHITE);
			}
			else if (boss1Hp >= 4800)
			{
				Novice::DrawSprite(10, 0, HPsprite[2], 1, 1, 0.0f, WHITE);
			}
			else if (boss1Hp >= 4700)
			{
				Novice::DrawSprite(10, 0, HPsprite[3], 1, 1, 0.0f, WHITE);
			}
			else if (boss1Hp >= 4600)
			{
				Novice::DrawSprite(10, 0, HPsprite[4], 1, 1, 0.0f, WHITE);
			}
			else if (boss1Hp >= 4500)
			{
				Novice::DrawSprite(10, 0, HPsprite[5], 1, 1, 0.0f, WHITE);
			}
			else if (boss1Hp >= 4400)
			{
				Novice::DrawSprite(10, 0, HPsprite[6], 1, 1, 0.0f, WHITE);
			}
			else if (boss1Hp >= 4300)
			{
				Novice::DrawSprite(10, 0, HPsprite[7], 1, 1, 0.0f, WHITE);
			}
			else if (boss1Hp >= 4200)
			{
				Novice::DrawSprite(10, 0, HPsprite[8], 1, 1, 0.0f, WHITE);
			}
			else if (boss1Hp >= 4100)
			{
				Novice::DrawSprite(10, 0, HPsprite[9], 1, 1, 0.0f, WHITE);
			}
			else if (boss1Hp >= 4000)
			{
				Novice::DrawSprite(10, 0, HPsprite[10], 1, 1, 0.0f, WHITE);
			}
			else if (boss1Hp >= 3900)
			{
				Novice::DrawSprite(10, 0, HPsprite[11], 1, 1, 0.0f, WHITE);
			}
			else if (boss1Hp >= 3800)
			{
				Novice::DrawSprite(10, 0, HPsprite[12], 1, 1, 0.0f, WHITE);
			}
			else if (boss1Hp >= 3700)
			{
				Novice::DrawSprite(10, 0, HPsprite[13], 1, 1, 0.0f, WHITE);
			}
			else if (boss1Hp >= 3600)
			{
				Novice::DrawSprite(10, 0, HPsprite[14], 1, 1, 0.0f, WHITE);
			}
			else if (boss1Hp >= 3500)
			{
				Novice::DrawSprite(10, 0, HPsprite[15], 1, 1, 0.0f, WHITE);
			}
			else if (boss1Hp >= 3400)
			{
				Novice::DrawSprite(10, 0, HPsprite[16], 1, 1, 0.0f, WHITE);
			}
			else if (boss1Hp >= 3300)
			{
				Novice::DrawSprite(10, 0, HPsprite[17], 1, 1, 0.0f, WHITE);
			}
			else if (boss1Hp >= 3200)
			{
				Novice::DrawSprite(10, 0, HPsprite[18], 1, 1, 0.0f, WHITE);
			}
			else if (boss1Hp >= 3100)
			{
				Novice::DrawSprite(10, 0, HPsprite[19], 1, 1, 0.0f, WHITE);
			}
			else if (boss1Hp >= 3000)
			{
				Novice::DrawSprite(10, 0, HPsprite[20], 1, 1, 0.0f, WHITE);
			}
			else if (boss1Hp >= 2900)
			{
				Novice::DrawSprite(10, 0, HPsprite[21], 1, 1, 0.0f, WHITE);
			}
			else if (boss1Hp >= 2800)
			{
				Novice::DrawSprite(10, 0, HPsprite[22], 1, 1, 0.0f, WHITE);
			}
			else if (boss1Hp >= 2700)
			{
				Novice::DrawSprite(10, 0, HPsprite[23], 1, 1, 0.0f, WHITE);
			}
			else if (boss1Hp >= 2600)
			{
				Novice::DrawSprite(10, 0, HPsprite[24], 1, 1, 0.0f, WHITE);
			}
			else if (boss1Hp >= 2500)
			{
				Novice::DrawSprite(10, 0, HPsprite[25], 1, 1, 0.0f, WHITE);
			}
			else if (boss1Hp >= 2400)
			{
				Novice::DrawSprite(10, 0, HPsprite[26], 1, 1, 0.0f, WHITE);
			}
			else if (boss1Hp >= 2300)
			{
				Novice::DrawSprite(10, 0, HPsprite[27], 1, 1, 0.0f, WHITE);
			}
			else if (boss1Hp >= 2200)
			{
				Novice::DrawSprite(10, 0, HPsprite[28], 1, 1, 0.0f, WHITE);
			}
			else if (boss1Hp >= 2100)
			{
				Novice::DrawSprite(10, 0, HPsprite[29], 1, 1, 0.0f, WHITE);
			}
			else if (boss1Hp >= 2000)
			{
				Novice::DrawSprite(10, 0, HPsprite[30], 1, 1, 0.0f, WHITE);
			}
			else if (boss1Hp >= 1900)
			{
				Novice::DrawSprite(10, 0, HPsprite[31], 1, 1, 0.0f, WHITE);
			}
			else if (boss1Hp >= 1800)
			{
				Novice::DrawSprite(10, 0, HPsprite[32], 1, 1, 0.0f, WHITE);
			}
			else if (boss1Hp >= 1700)
			{
				Novice::DrawSprite(10, 0, HPsprite[33], 1, 1, 0.0f, WHITE);
			}
			else if (boss1Hp >= 1600)
			{
				Novice::DrawSprite(10, 0, HPsprite[34], 1, 1, 0.0f, WHITE);
			}
			else if (boss1Hp >= 1500)
			{
				Novice::DrawSprite(10, 0, HPsprite[35], 1, 1, 0.0f, WHITE);
			}
			else if (boss1Hp >= 1400)
			{
				Novice::DrawSprite(10, 0, HPsprite[36], 1, 1, 0.0f, WHITE);
			}
			else if (boss1Hp >= 1300)
			{
				Novice::DrawSprite(10, 0, HPsprite[37], 1, 1, 0.0f, WHITE);
			}
			else if (boss1Hp >= 1200)
			{
				Novice::DrawSprite(10, 0, HPsprite[38], 1, 1, 0.0f, WHITE);
			}
			else if (boss1Hp >= 1100)
			{
				Novice::DrawSprite(10, 0, HPsprite[39], 1, 1, 0.0f, WHITE);
			}
			else if (boss1Hp >= 1000)
			{
				Novice::DrawSprite(10, 0, HPsprite[40], 1, 1, 0.0f, WHITE);
			}
			else if (boss1Hp >= 900)
			{
				Novice::DrawSprite(10, 0, HPsprite[41], 1, 1, 0.0f, WHITE);
			}
			else if (boss1Hp >= 800)
			{
				Novice::DrawSprite(10, 0, HPsprite[42], 1, 1, 0.0f, WHITE);
			}
			else if (boss1Hp >= 700)
			{
				Novice::DrawSprite(10, 0, HPsprite[43], 1, 1, 0.0f, WHITE);
			}
			else if (boss1Hp >= 600)
			{
				Novice::DrawSprite(10, 0, HPsprite[44], 1, 1, 0.0f, WHITE);
			}
			else if (boss1Hp >= 500)
			{
				Novice::DrawSprite(10, 0, HPsprite[45], 1, 1, 0.0f, WHITE);
			}
			else if (boss1Hp >= 400)
			{
				Novice::DrawSprite(10, 0, HPsprite[46], 1, 1, 0.0f, WHITE);
			}
			else if (boss1Hp >= 300)
			{
				Novice::DrawSprite(10, 0, HPsprite[47], 1, 1, 0.0f, WHITE);
			}
			else if (boss1Hp >= 200)
			{
				Novice::DrawSprite(10, 0, HPsprite[48], 1, 1, 0.0f, WHITE);
			}
			else if (boss1Hp >= 100)
			{
				Novice::DrawSprite(10, 0, HPsprite[49], 1, 1, 0.0f, WHITE);
			}
			//デバッグ用
			Novice::ScreenPrintf(0, 0, "bulletTimer = %d", bulletTimer);

			//描画処理ここまで

			break;

		case GAMEOVER:

			//更新処理ここから

			if (keys[DIK_SPACE] && !preKeys[DIK_SPACE])
			{
				scene = TITLE;
			}

			//更新処理ここまで


			//描画処理ここから

			Novice::ScreenPrintf(0, 0, "scene = %d", scene);

			//描画処理ここまで

			break;

			//ゲームクリア画面
		case GAMECLEAR:

			//更新処理ここから

			if (keys[DIK_SPACE] && !preKeys[DIK_SPACE])
			{
				scene = TITLE;
			}

			//更新処理ここまで


			//描画処理ここから

			Novice::ScreenPrintf(0, 0, "scene = %d", scene);

			//描画処理ここまで

			break;
		}

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0)
		{
			break;
		}
	}
	// ライブラリの終了
	Novice::Finalize();
	return 0;
}