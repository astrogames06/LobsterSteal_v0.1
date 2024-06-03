#include <raylib.h>
#include <iostream>
#include <random>
#include <chrono>



#define REDTR Color{ 200, 21, 25, 105 }

Texture2D LobsterRight;
Texture2D LobsterLeft;

Texture2D Room1Wall;
Texture2D Room2Wall;
Texture2D Room3Wall;
Texture2D Room4Wall;
Texture2D Room5Wall;

Texture2D itemsTexture;
Texture2D ratTexture;
Texture2D sobTexture;
Texture2D Misc;

Texture2D Bowl;
Texture2D Butter;
Texture2D Cracker;
Texture2D Cup;
Texture2D Fork;
Texture2D Granade;
Texture2D Pen;
Texture2D Plate;
Texture2D Stone;
Texture2D Picture;
Texture2D KitchenKnife;
Texture2D ShedTexture;

Texture2D KitchenTexture;
Texture2D LampTexture1;
Texture2D LampTexture2;
Texture2D GrassTexture;
Texture2D TrashcanTexture1;
Texture2D TrashcanTexture2;
Texture2D TrashcanTexture3;
Texture2D DeskTexture1;
Texture2D DeskTexture2;
Texture2D Cloud1;
Texture2D Cloud2;
Texture2D Cloud3;
Texture2D Sun;
Texture2D WardobleTexture1;
Texture2D WardobleTexture2;
Texture2D BackgroundTexture;


const int screenWidth = 1500;
const int screenHeight = 800;
bool paused = false;
float gravity = 5; 

int tmp_int1;
int tmp_int2;
float tmp_float1;
float tmp_float2;

class Menu {
public:
	int GameState = 2; // 0 - Pause, 1 - Game, 2 - Menu, 3 - Game Over Screen 4 - Victory Screen
	int GameOverTimer = 1000;
	int MenuSelect = 1;
	int seconds2end = 10;
	int milis2end = 0;
	float timer1 = 10;
	float * timer1pointer = &timer1;
	bool GameExit = false;
	bool resp = false;





	void StartMenu()
	{
		if (IsKeyPressed(KEY_S) && MenuSelect > 1)
		{
			MenuSelect = MenuSelect - 1;
		}
		if (IsKeyPressed(KEY_W) && MenuSelect < 3)
		{
			MenuSelect = MenuSelect + 1;
		}


		ClearBackground(GRAY);

		DrawText("Lobster Lobster Lobster Lobster", screenWidth / 2, screenHeight / 10, 40, WHITE);
		//printf("Switch %d", MenuSelect);
		switch(MenuSelect)
		{
			case(3):
			{
				DrawRectangle(screenWidth / 2, screenHeight * 4 / 8, 700, 40, BLACK);
				DrawText("Start", screenWidth / 2, screenHeight * 4 / 8, 40, WHITE);
				if (IsKeyPressed(KEY_SPACE))
				{
					GameState = 1;
					resp = true;
				}
				break;
			}
			case(2):
			{
				DrawRectangle(screenWidth / 2, screenHeight * 5 / 8, 700, 40, BLACK);
				DrawText("Options are supposed to be here", screenWidth / 2, screenHeight * 5 / 8, 40, WHITE);
				if (IsKeyPressed(KEY_SPACE))
				{
				}
				break;
			}
			case(1):
			{
				DrawRectangle(screenWidth / 2, screenHeight * 6 / 8, 700, 40, BLACK);
				DrawText("Exit", screenWidth / 2, screenHeight * 6 / 8, 40, WHITE);
				if (IsKeyPressed(KEY_SPACE))
				{
					GameExit = true;
				}
				
				break;
			}
		default:
			break;
		}

	}

	void GameOver()
	{
		*timer1pointer = *timer1pointer - GetFrameTime();

		if (*timer1pointer < 0)
		{
			GameState = 2;
			timer1 = 10;
			DrawRectangle(0, 0, screenWidth, screenHeight, REDTR);
			DrawText("Game Over", screenWidth / 2, screenHeight / 10, 40, WHITE);
			DrawText("0", screenWidth / 2, screenHeight / 2, 80, WHITE);
			DrawText("Press space to respawn", screenWidth / 2, screenHeight * 9 / 10, 5, WHITE);
		}

		DrawRectangle(0, 0, screenWidth, screenHeight, REDTR);
		DrawText("Game Over", screenWidth / 2, screenHeight / 10, 40, WHITE);
		DrawText(TextFormat("%f", *timer1pointer), screenWidth / 2, screenHeight / 2, 80, WHITE);
		DrawText("Press space to respawn", screenWidth / 2, screenHeight * 9 / 10, 5, WHITE);

	}
};

Menu menu;

class Player {
public:

	float PlayerPosX = 18000;
	float PlayerPosY = 400;
	float PlayerAccelX = 0;
	float PlayerAccelY = 0;
	float PlayerVelX = 0;
	float PlayerVelY = 0;
	float PlayerDragFactor = 0.1;
	float PlayerMass = PlayerMassBaseline;
	float PlayerMassBaseline = 10;
	float EquipmentMass = 1;
	float PlayerBreakingAccelAction = 10;
	float PlayerAccelAction = 3;
	float PlayerVelJump = 50;
	float cooldown = 10;
	float PlayerSize = 10;
	float PlayerAttackRange = 200;
	float Damage = 5;

	bool PlayerActionRenderRequired = false;
	bool BottomCollision = false;
	bool RightCollision = false;
	bool LeftCollision = false;
	bool RightItemCollision = false;
	bool LeftItemCollision = false;
	bool BottomItemCollision = false;
	bool isOnTopItem = false;
	bool PlayerActionHappend = false;
	bool PlayerCanPerformAction = true;
	bool IsPlayerDead = false;
	bool PlayerFacingRight = false;
	bool Walking = true;

	void PlayerUpdate()
	{

		PlayerPosX = PlayerPosX + PlayerVelX;

		PlayerVelX = PlayerVelX + PlayerAccelX;

		PlayerAccelX = PlayerAccelX / 2;

		if (PlayerVelX < 0)
		{
			PlayerAccelX = PlayerAccelX - PlayerDragFactor * PlayerVelX;
		}

		if (PlayerVelX > 0)
		{
			PlayerAccelX = PlayerAccelX - PlayerDragFactor * PlayerVelX;
		}

		PlayerPosY = PlayerPosY - PlayerVelY;

		PlayerVelY = PlayerVelY + PlayerAccelY;

		if (!BottomCollision || !BottomItemCollision) 
		{ 
			PlayerAccelY = PlayerAccelY - gravity; 
		}

		PlayerAccelY = PlayerAccelY / 2;

		if (PlayerVelY < 0)
		{
			PlayerAccelY = PlayerAccelY - PlayerDragFactor * PlayerVelY;
		}

		if (PlayerVelY > 0)
		{
			PlayerAccelY = PlayerAccelY - PlayerDragFactor * PlayerVelY;
		}


		//printf("Player Accel: %lf \n", PlayerAccelY);
		//printf("Player Vel: %lf \n", PlayerVelY);


		//Action Update

		if (!PlayerCanPerformAction)
		{
			cooldown = cooldown - 1;
			if (cooldown <= 0)
			{
				cooldown = 30;
				PlayerCanPerformAction = true;
			}
		}

		if (PlayerPosY > 700)
		{
			IsPlayerDead = true;
		}
		
		if (IsPlayerDead)
		{
			menu.GameState = 3;
		}

		if (PlayerVelX > 0.1)
		{
			PlayerFacingRight = true;
			
		}
		if (PlayerVelX < -0.1)
		{
			PlayerFacingRight = false;
		}




	}

	void PlayerMovement()
	{

		if (IsKeyDown(KEY_D) && !RightCollision && !RightItemCollision) 
		{
			PlayerAccelX = PlayerAccelX + PlayerAccelAction;

		} 

		if (IsKeyDown(KEY_A) && !LeftCollision && !LeftItemCollision)
		{
			PlayerAccelX = PlayerAccelX - PlayerAccelAction;
		}

		if (IsKeyDown(KEY_W) && (BottomCollision || BottomItemCollision))
		{
			BottomItemCollision = false;
			BottomCollision = false;
			PlayerVelY = PlayerVelJump;
		}

	}

	void PlayerAction()
	{
		if (PlayerCanPerformAction)
		{
			PlayerActionRenderRequired = true;
			PlayerActionHappend = true;
			PlayerCanPerformAction = false;
		}
	}

	void PlayerCollision()
	{
		if (CheckCollisionCircleRec(Vector2{ PlayerPosX, PlayerPosY }, 20, Rectangle{ 0,400,99999,999 }))
		{
			BottomCollision = true;
			if (PlayerVelY < 0)
			{
				PlayerVelY = 0;
				PlayerAccelY = 0;
			}
		}
		else
		{
			BottomCollision = false;
		}

	}

	void PlayerRespawn()
	{
		PlayerPosX = 17500;
		PlayerPosY = 350;
		menu.timer1 = 3;
		IsPlayerDead = false;
		menu.GameState = 1;
		
	}


	void PlayerRender()
	{
		DrawCircle(PlayerPosX, PlayerPosY, PlayerSize, RED);
		if (!PlayerFacingRight)
		{
			DrawTexture(LobsterLeft, PlayerPosX-50, PlayerPosY-20, WHITE);
			//printf("Left");
			//PlaySound(Walking);
		}
		else {
			DrawTexture(LobsterRight, PlayerPosX-50, PlayerPosY-20, WHITE);
			//printf("Right");
			//PlaySound(Walking);
		}
	}

	void PlayerActionRender()
	{

			DrawCircle(PlayerPosX, PlayerPosY, PlayerSize, WHITE);
			PlayerActionRenderRequired = false;
	}


};

Player player;

class Equipment {
public:

	bool ErrorFullSlots = false;
	bool Slot1Full = false;
	bool Slot2Full = false;
	float EquipmentHeight = 200;
	float PlacementHeight = 20;
	int SlotPositionX = 0;
	int SlotPositionY = screenHeight - EquipmentHeight + PlacementHeight + 20;
	float SlotSize = 160;
	int floor = -2;
	int room = 3;
	int ItemSize = 120;
	bool ItemCollision = false;


	typedef struct Slot {

		bool IsSlotFull;
		int ItemID;
		Color color;
		int ItemPositionX;
		int ItemPositionY;
	} SlotA;

	SlotA slot[3] = {
	{ true, 1, RED, screenWidth / 8 - 80, SlotPositionY},
	{ false, 0, WHITE, screenWidth * 2 / 8 - 80, SlotPositionY},
	{ false, 0, WHITE, screenWidth * 3 / 8 - 80, SlotPositionY},
	};

	struct Items
	{
		int ID;
		int mass;
		Color color;
	};

	Items ItemMenu[3]{
		{1, 5, BLUE},
		{2, 2, YELLOW},
		{3, 10, BLACK},
	};



	int SpecialItemInUse = -1;
	int ItemAmount = 15;
	float ItemPositionX[100] =       { 500,   1000,   1200 , 1700, 500,    900, 1100, 1300, 1400, 1600, 300};
	float ItemPositionY[100] =       { 80 ,     0 ,     0 ,    0,    0,       0, 0,0,0,0,0,0,0,0};
	float ItemAccelX[100] =          { 0 ,     0 ,     0 ,    0,    0,       0,0,0,0,0,0,0,0,0,0};
	float ItemAccelY[100] =          { 0 ,     0 ,     0 ,    0,    0,       0,0,0,0,0,0,0};
	float ItemVelocityX[100] =       { 0 ,     0 ,     0 ,    0,    0,       0,0,0,0,0,0,0};
	float ItemVelocityY[100] =       { 0 ,     0 ,     0 ,    0,    0 ,      0,0,0,00,0,0,0,0};
	Color ItemColor[100] =           { GREEN, BLUE , BROWN , RED, DARKGRAY, GRAY,GRAY,GRAY,GRAY,GRAY, DARKGREEN};
	int ItemIDArray[100] =           { 0 , 1 , 2 , 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	char ItemName[100][20] =         {"Picture","Pencil" , "Table", "Cracker", "Knife","Butter Knife","Fork","Plate", "Cup", "Military Granade" };
	bool ItemPlayerCollision[100] =  { false ,  false , true , false,  false, false, false, false, false , false, false};
	bool ItemBottomCollision[100] =  { false ,  false , false , false, false, false, false, false, false , false, false};

	struct ItemStats
	{
		int  ItemIDArray2;
		char Name[20];
		int  Mass;
		int  Drag;
		bool ItemStayInPos;
		bool ItemHasDamage;
		float  Cooldown;
		int  RefferenceType;
		int  RefferenceID;
		int  ItemThrowDamage;
		bool IsItemSpecial;
		float Timer1;
		float Timer2;
		bool ItemSpecialBool;
	};

	ItemStats ItemStats[100]
	{
		{0, "Picture", 5, 10, true, false, 0, 0, 0, 0, false},
		{1, "Pencil",  5, 1, false, false, 0, 0, 0, 0, false},
		{2, "Table", 90, 10, false, false, 0, 0, 0, 0, false},
		{3, "Cracker", 5, 10, false, false, 0, 0, 0, 0, false},
		{4, "Knife", 5, 10, false, true, 40, 0, 1, 15, false},
		{5, "Butter Knife", 5, 10, false, false, 0, 0, 0, 0, false},
		{6, "Fork", 5, 10, false, false, 0, 0, 0, 0, false},
		{7, "Plate", 5, 10, false, false, 0, 0, 0, 0, false},
		{8, "Cup", 5, 10, false, false, 0, 0, 0, 0, false},
		{9, "Bowl", 5, 10, false, false, 0, 0, 0, 0, false},
		{10, "Military Granade", 5, 10, false, false, 3, 0, 0, 1, true, 1, 1, false},
	};

	struct ExplosiveStats
	{
		int  ItemID;
		float	ItemDamage;
		float	ItemRange;
		float	PressurePowerFactor;
		float	Fragments;
		float	FragmentDamage;
	};

	ExplosiveStats explosiveStat[100]
	{
		{10, 200, 600, 10, 6, 5}, //Military Granade
	};

	struct WeaponStats
	{
		int  ItemID;
		float	ItemDamage;
		float	ItemRange;
		float	Cooldown;
		Rectangle	AttackArea;
	};

	WeaponStats weaponStat[10]
	{
		{10, 5,  15, 5,  {0, 50 , 100, 100}}, //Knife
		{10, 30, 20, 15, {0, 50 , 100, 100}}, //Better Knife
	};


	void ItemUse()
	{
		if(IsKeyDown(KEY_X))
		{
			
			for (int i = 1; i < 2; i++)
			{
				printf("Check1 %d", slot[i].ItemID);
				int selectedSlot = i;
				if (ItemStats[slot[i].ItemID].IsItemSpecial)
				{
					printf("Check2");
					switch (slot[i].ItemID)
					{
					case(10):
					{
						printf("Check3\n");
						SpecialItemInUse = 10;
						slot[selectedSlot].IsSlotFull = false;
						ItemPositionX[slot[selectedSlot].ItemID] = player.PlayerPosX;
						ItemPositionY[slot[selectedSlot].ItemID] = player.PlayerPosY - 40;
						if (player.PlayerFacingRight)
						{
							ItemVelocityX[slot[selectedSlot].ItemID] = 40;
						}
						else {
							ItemVelocityX[slot[selectedSlot].ItemID] = -40;
						}
						ItemVelocityY[slot[selectedSlot].ItemID] = -20;
						slot[i].ItemID = -1;
					}
					}


					break;
				}
			}
		}
		if (ItemStats[10].ItemSpecialBool)
		{
			ItemStats[SpecialItemInUse].ItemSpecialBool = false;
			ItemPositionY[SpecialItemInUse] = 9999;
			SpecialItemInUse = -1;

		}

		if (SpecialItemInUse == 10)
		{
			
			ItemStats[SpecialItemInUse].Cooldown = ItemStats[SpecialItemInUse].Cooldown - GetFrameTime();
			printf("Check4 %f\n", ItemStats[SpecialItemInUse].Cooldown);
			if (ItemStats[SpecialItemInUse].Cooldown < 0)
			{
				ItemStats[SpecialItemInUse].ItemSpecialBool = true;
				
				printf("Check5");

				
			}
		}


	}

	void ItemDrop(int selectedSlot)
	{
		if (slot[selectedSlot].IsSlotFull == true)
		{
			slot[selectedSlot].IsSlotFull = false;
			ItemPositionX[slot[selectedSlot].ItemID] = player.PlayerPosX;
			ItemPositionY[slot[selectedSlot].ItemID] = player.PlayerPosY;
			ItemVelocityX[slot[selectedSlot].ItemID] = player.PlayerVelX;
			ItemVelocityY[slot[selectedSlot].ItemID] = player.PlayerVelY;
		}
		if (ItemStats[slot[selectedSlot].ItemID].ItemHasDamage)
		{
			player.Damage = 5;
		}
	}

	void SlotFullError()
	{
		int abd = 0;
	}

	void ItemPickup()
	{
		for (int i = 0; i < ItemAmount; i++)
		{
			if (CheckCollisionCircleRec(Vector2{ player.PlayerPosX, player.PlayerPosY }, 40, Rectangle{ItemPositionX[i], ItemPositionY[i], 120, 120}))
			{
				if (slot[1].IsSlotFull)
				{
					if (slot[2].IsSlotFull)
					{
						SlotFullError();
					}
					else {
						
						slot[2].IsSlotFull = true;
						slot[2].color = ItemColor[i];
						slot[2].ItemID = ItemIDArray[i];
						ItemPositionX[i] = 9999;
						ItemPositionY[i] = 3000;
						ItemStats[i].ItemStayInPos = false;
						
					}
				} else {
					slot[1].IsSlotFull = true;
					slot[1].color = ItemColor[i];
					slot[1].ItemID = ItemIDArray[i];
					ItemPositionX[i] = 9999;
					ItemPositionY[i] = 3000;
					ItemStats[i].ItemStayInPos = false;
				}

				if (ItemStats[i].ItemHasDamage)
				{
					player.Damage = weaponStat[ItemStats[i].RefferenceID].ItemDamage;
				}

			}


		}
	}

	void ItemUpdate()
	{
		for (int i = 0; i < ItemAmount; i++)
		{
			if (!ItemBottomCollision[i])
			{
				if (!ItemStats[i].ItemStayInPos)
				{
					ItemPositionX[i] = ItemPositionX[i] + ItemVelocityX[i];
					ItemVelocityX[i] = ItemVelocityX[i] / 1.1;

					ItemPositionY[i] = ItemPositionY[i] + ItemVelocityY[i];
					ItemVelocityY[i] = ItemVelocityY[i] + gravity;
					ItemVelocityY[i] = ItemVelocityY[i] / 1.2;
				}
			}

			if (CheckCollisionCircleRec(Vector2{ ItemPositionX[i], ItemPositionY[i] + ItemSize/2 }, 30, Rectangle{ 100,400,99999,200 }))
			{
				ItemVelocityY[i] = 0;
				ItemBottomCollision[i] = true;

			}
			else {
				ItemBottomCollision[i] = false;
			}

			
			if (player.PlayerPosY < ItemPositionY[i] + 20)
			{
				player.isOnTopItem = true;
				//printf("is on top");
			}
			else {
				player.isOnTopItem = false;
			}

			if (ItemPlayerCollision[i])
			{
				if (CheckCollisionCircleRec(Vector2{ player.PlayerPosX, player.PlayerPosY}, player.PlayerSize + 5, Rectangle{ ItemPositionX[i], ItemPositionY[i], (float)ItemSize/2, (float)ItemSize/2, }))
				{
					ItemCollision = true;



					//printf("Collision positive ");
					if (!player.isOnTopItem)
					{
						if (player.PlayerPosX > ItemPositionX[i] + ItemSize / 2)
						{

							player.LeftItemCollision = true;
						}
						else {
							player.LeftItemCollision = false;
						}

						if (player.PlayerPosX < ItemPositionX[i] + ItemSize / 2)
						{
							player.RightItemCollision = true;
						}
						else {
							player.RightItemCollision = false;
						}
					}

					if (player.PlayerPosY + player.PlayerSize/2 > ItemPositionY[i] && player.PlayerPosX > ItemPositionX[i] && player.PlayerPosX < ItemPositionX[i] + ItemSize/2)
					{
						printf("Bottom Collision");

						player.BottomItemCollision = true;
						if (player.PlayerVelY < 0)
						{
							player.PlayerAccelY = 0;
							player.PlayerVelY = 0;

							//Up to the item surface
							player.PlayerPosY = ItemPositionY[i] - player.PlayerSize / 2;


						}
					}
					else {
						player.BottomItemCollision = false;
					}

				}
				else {
					if (ItemCollision)
					{
						ItemCollision = false;
						player.RightItemCollision = false;
						player.LeftItemCollision = false;
						player.BottomItemCollision = false;
					}
				}
			}
		}
	}

	
	bool ExploRender = false;
	float ExploRendertime1;
	float ExploRendertime2;
	float ExplosionSize = 30;

	void ExplosiveDamageExplosionDetection(Vector2 SubjectPos, Vector2 ExplosionPos, float Armour, float ExplosionRadius, float ExplosionDamage, float PressureDecreaseFactor, float FragmentationDamage, int AmountofFragments, float * HealthPointer)
	{
		bool SomethingHappen = false;
		//Estimate if object is within explosion
		float DistanceFromExplosion = 0;
		DistanceFromExplosion = pow(pow(((SubjectPos.x - ExplosionPos.x)),2) + (pow((ExplosionPos.y - SubjectPos.y),2)), 0.5);
		printf("Test1: %f\n", pow(pow(SubjectPos.x - ExplosionPos.x, 2), 0.5));
		printf("Test2: %f\n", pow(pow((SubjectPos.y - ExplosionPos.y), 2), 0.5));
		printf("Test3: %f\n", pow(pow(ExplosionPos.x, 2), 0.5));
		printf("Distance calculated: %f", DistanceFromExplosion);

		if (DistanceFromExplosion < ExplosionRadius)
		{
			//Estimate Pressure Damage
			long float DamageRecieved;
			DamageRecieved = ((((ExplosionDamage * (DistanceFromExplosion + 10) * pow(PressureDecreaseFactor, 2))) / (Armour/10)) / pow((DistanceFromExplosion + 10), 2)) - 1;
			printf("DamageDealt: %f\n", DamageRecieved);
			*HealthPointer = *HealthPointer - DamageRecieved;
			printf("Health left: &f\n", *HealthPointer);

			SomethingHappen = true;

		}
		else
		{
			printf("Object not affected\n");
			SomethingHappen = true;
		}
		if (SomethingHappen)
		{
			ExploRender = true;
			tmp_float1 = ExplosionPos.x;                                    
			tmp_float2 = ExplosionPos.y;
			tmp_int1 = SpecialItemInUse;
			ExploRendertime1 = ItemStats[tmp_int1].Timer1;
			ExploRendertime2 = ItemStats[tmp_int1].Timer1;
			SomethingHappen = false;
		}

			

		


	}

	
	void ExplosionRender()
	{
		

		if (ExploRender)
		{
			printf("explorender called %f\n", ExploRendertime1);
			ExploRendertime1 = ExploRendertime1 - GetFrameTime();
			DrawCircleLines(tmp_float1, tmp_float2, ExplosionSize, WHITE);

			if (ExploRendertime1 < ExploRendertime2)
			{
				ExploRendertime2 = ExploRendertime2 - 0.1;

				ExplosionSize = ExplosionSize + ExplosionSize * 60 * (1/ExplosionSize);
				printf("explo render called, %d, %f %f, %f, %f\n", ExplosionSize, ItemStats[tmp_int1].Timer1, ItemStats[tmp_int1].Timer2,tmp_float1, tmp_float2);
				
				//DrawTexture(ExplosionTextures[0], 0, 0, 0, RED);

			}
			if (ExploRendertime2 < 0)
			{
				ExploRender = false;
			}
		}
	}











	void ItemRender()
	{
		/*
		for (int i = 0; i < ItemAmount; i++)
		{
			DrawRectangle(ItemPositionX[i], ItemPositionY[i], ItemSize / 2, ItemSize / 2, ItemColor[i]);
			DrawTexturePro(itemsTexture, Rectangle{ 1, 1, 50, 60 }, Rectangle{ ItemPositionX[10], ItemPositionY[10], 50, 50}, Vector2{0 , 0}, 0, WHITE);
		}
		*/
		DrawTexture(Picture, ItemPositionX[0], ItemPositionY[0], WHITE);
		DrawTexture(KitchenKnife, ItemPositionX[4], ItemPositionY[4], WHITE);
		DrawTexture(Stone, ItemPositionX[2], ItemPositionY[2], WHITE);
		DrawTexture(Fork, ItemPositionX[6], ItemPositionY[6], WHITE);
		DrawTexture(Plate, ItemPositionX[7], ItemPositionY[7], WHITE);
		DrawTexture(Cup, ItemPositionX[8], ItemPositionY[8], WHITE);
		DrawTexture(Cracker, ItemPositionX[3], ItemPositionY[3], WHITE);
		DrawTexture(Pen, ItemPositionX[1], ItemPositionY[1], WHITE);
		DrawTexture(Butter, ItemPositionX[5], ItemPositionY[5], WHITE);
		DrawTexture(Bowl, ItemPositionX[9], ItemPositionY[9], WHITE);
		DrawTexture(Granade, ItemPositionX[10], ItemPositionY[10], WHITE);
	}








	Rectangle RecEquipment{ 0, screenHeight - EquipmentHeight, screenWidth, screenHeight };
	Rectangle RecWeaponSlot{ screenWidth / 8 - 100, screenHeight - EquipmentHeight + PlacementHeight, SlotSize, SlotSize };
	Rectangle RecItemSlot1{ screenWidth * 2 / 8 - 100, screenHeight - EquipmentHeight + PlacementHeight, SlotSize, SlotSize };
	Rectangle RecItemSlot2{ screenWidth * 3 / 8 - 100, screenHeight - EquipmentHeight + PlacementHeight, SlotSize, SlotSize };
	Rectangle RecTester{ screenWidth * 4 / 8 - 100, screenHeight - EquipmentHeight + PlacementHeight, SlotSize, SlotSize };
	Rectangle RecMiniMap{ screenWidth * 5 / 8 - 100, screenHeight - EquipmentHeight + PlacementHeight, SlotSize, SlotSize };
	Rectangle RecRoom0{ screenWidth * 5 / 8 - 80, screenHeight - EquipmentHeight + PlacementHeight + 20, 60, 60 };    //Top Right Floor 0
	Rectangle RecRoom1{ screenWidth * 5 / 8 - 20, screenHeight - EquipmentHeight + PlacementHeight + 20, 60, 60 };    //Top Left  Floor 0
	Rectangle RecRoom2{ screenWidth * 5 / 8 - 80, screenHeight - EquipmentHeight + PlacementHeight + 80, 120, 60 };   //Bottom    Floor 0
	Rectangle RecRoom3{ screenWidth * 5 / 8 - 80, screenHeight - EquipmentHeight + PlacementHeight + 20, 120, 120 };  //Garden    Floor -2
	Rectangle RecRoom4{ screenWidth * 5 / 8 - 10, screenHeight - EquipmentHeight + PlacementHeight + 30, 40, 20 };    //Gardern House Floor -2
	Rectangle RecRoom0B{ screenWidth * 5 / 8 - 80, screenHeight - EquipmentHeight + PlacementHeight + 20, 50, 60 };    //Stairs up Floor 0
	Rectangle RecRoom5{ screenWidth * 5 / 8 - 30, screenHeight - EquipmentHeight + PlacementHeight + 20, 20, 120 };   //Corridor Floor 1
	Rectangle RecRoom6{ screenWidth * 5 / 8 - 80, screenHeight - EquipmentHeight + PlacementHeight + 80, 50, 60 };    //Kitchen Floor -2
	Rectangle RecRoom7{ screenWidth * 5 / 8 - 10, screenHeight - EquipmentHeight + PlacementHeight + 20, 50, 60 };    //Private room 1 Floor 1
	Rectangle RecRoom8{ screenWidth * 5 / 8 - 10, screenHeight - EquipmentHeight + PlacementHeight + 80, 50, 60 };    //Private room 2 Floor 1
	Rectangle RecRoom9{ screenWidth * 5 / 8 + 40, screenHeight - EquipmentHeight + PlacementHeight + 70, 20, 70 };    //Balcony        Floor 1
	Rectangle RecRoom10{ screenWidth * 5 / 8 - 80, screenHeight - EquipmentHeight + PlacementHeight + 80, 120, 60 };    //Living Room Floor 2
	Rectangle RecRoom11{ screenWidth * 5 / 8 - 20, screenHeight - EquipmentHeight + PlacementHeight + 20, 30, 60 };    //Private room 3 Floor 2
	Rectangle RecRoom12{ screenWidth * 5 / 8 + 10, screenHeight - EquipmentHeight + PlacementHeight + 20, 30, 60 };    //Bathroom Floor 2
	Rectangle RecRoom13{ screenWidth * 5 / 8 + 40, screenHeight - EquipmentHeight + PlacementHeight + 70, 20, 70 };   //Balcony2 Floor 2
	Rectangle RecRoom14{ screenWidth * 5 / 8 - 80, screenHeight - EquipmentHeight + PlacementHeight + 20, 120, 120 };    //Roof Floor 3
	Rectangle RecRoom15{ screenWidth * 5 / 8 - 80, screenHeight - EquipmentHeight + PlacementHeight + 20, 120, 120 };    // Floor -1
	Rectangle RecRoom16{ screenWidth * 5 / 8 - 40, screenHeight - EquipmentHeight + PlacementHeight + 20, 50, 35 };    // Floor -1
	Rectangle RecRoom17{ screenWidth * 5 / 8 - 30, screenHeight - EquipmentHeight + PlacementHeight + 140, 20, 20 };    // Floor -1



	void DrawEquipment()
	{

		DrawRectangleRec(RecEquipment, RAYWHITE);
		DrawRectangleRec(RecWeaponSlot, GRAY);
		DrawRectangleRec(RecItemSlot1, GRAY);
		DrawRectangleRec(RecItemSlot2, GRAY);
		DrawRectangleRec(RecTester, GRAY);
		DrawText(TextFormat("%1.0f", player.cooldown), screenWidth * 4 / 8 - 50, screenHeight - EquipmentHeight + PlacementHeight * 2, 50, BLACK);
		DrawText(TextFormat("%1.0f", player.EquipmentMass), screenWidth * 4 / 8 - 50, screenHeight - EquipmentHeight + PlacementHeight * 4, 50, BLACK);
		DrawRectangleRec(RecMiniMap, BLACK);

		switch (room)
		{
			case -2:
			{

				break;
			}
			case -1:
			{

				break;
			}
			case 0:
			{
				DrawRectangleRec(RecRoom0, ORANGE);
				break;}
			case 1:
			{
				DrawRectangleRec(RecRoom1, ORANGE);
				break;}
			case 2:
			{
				DrawRectangleRec(RecRoom2, ORANGE);
				break;
			}
			case 3:
			{
				DrawRectangleRec(RecRoom3, ORANGE);
				DrawRectangleRec(RecRoom4, BLACK);
				break;
			}
			case 4:
			{
				DrawRectangleRec(RecRoom4, ORANGE);
				break;
			}
			case 5:
			{
				DrawRectangleRec(RecRoom5, ORANGE);
				break;
			}
			case 6:
			{
				DrawRectangleRec(RecRoom6, ORANGE);
				break;}
			case 7:
			{
				DrawRectangleRec(RecRoom7, ORANGE);
				break;}
			case 8:
			{
				DrawRectangleRec(RecRoom8, ORANGE);
				break;}
			case 9:
			{
				DrawRectangleRec(RecRoom9, ORANGE);
				break;}
			case 10:
			{
				DrawRectangleRec(RecRoom10, ORANGE);
				break;}
			case 11:
			{
				DrawRectangleRec(RecRoom11, ORANGE);
				break;}
			case 12:
			{
				DrawRectangleRec(RecRoom12, ORANGE);
				break;}
			case 13:
			{
				DrawRectangleRec(RecRoom13, ORANGE);
				break;}
			case 14:
			{
				DrawRectangleRec(RecRoom14, ORANGE);
				break;}
			case 15:
			{
				DrawRectangleRec(RecRoom15, ORANGE);
				DrawRectangleRec(RecRoom16,  BLACK);
				break;}
			case 16:
			{
				DrawRectangleRec(RecRoom16, ORANGE);
				break;}
			case 17:
			{
				DrawRectangleRec(RecRoom17, ORANGE);
				break;}
		}
		switch (floor)
		{
		case -2:
		{
			DrawRectangleLinesEx(RecRoom3, 5, WHITE);
			DrawRectangleLinesEx(RecRoom4, 5, WHITE);
			break;
		}
		case 0:			
			{
			DrawRectangleLinesEx(RecRoom0, 5, WHITE);
			DrawRectangleLinesEx(RecRoom1, 5, WHITE);
			DrawRectangleLinesEx(RecRoom2, 5, WHITE);
				break;
			}
		case 1:
		{
			DrawRectangleLinesEx(RecRoom0B, 5, WHITE);
			DrawRectangleLinesEx(RecRoom5, 5, WHITE);
			DrawRectangleLinesEx(RecRoom6, 5, WHITE);
			DrawRectangleLinesEx(RecRoom7, 5, WHITE);
			DrawRectangleLinesEx(RecRoom8, 5, WHITE);
			DrawRectangleLinesEx(RecRoom9, 2, WHITE);
			break;
		}
		case 2:
		{
			DrawRectangleLinesEx(RecRoom0, 5, WHITE);
			DrawRectangleLinesEx(RecRoom10, 5, WHITE);
			DrawRectangleLinesEx(RecRoom11, 5, WHITE);
			DrawRectangleLinesEx(RecRoom12, 5, WHITE);
			DrawRectangleLinesEx(RecRoom13, 2, WHITE);
			break;
		}
		case 3:
		{
			DrawRectangleLinesEx(RecRoom14, 5, WHITE);
			break;
		}
		case -1:
		{
			DrawRectangleLinesEx(RecRoom15, 5, WHITE);
			DrawRectangleLinesEx(RecRoom16, 5, WHITE);
			DrawRectangleLinesEx(RecRoom17, 5, WHITE);
			break;
		}




		default:
			break;
		}


		for (int i = 1; i < 3; i++)
		{
			int test = slot[i].ItemID;

			if (slot[i].IsSlotFull == true)
			{
				switch (test)
				{
					case 0:
					{
						DrawTexture(Picture, slot[i].ItemPositionX, slot[i].ItemPositionY, WHITE);
						break;
					}
					case 1:
					{
						DrawTexture(Pen, slot[i].ItemPositionX, slot[i].ItemPositionY, WHITE);
						break;
					}
					case 2:
					{
						DrawTexture(Stone, slot[i].ItemPositionX, slot[i].ItemPositionY, WHITE);
						break;
					}
					case 3:
					{
						DrawTexture(Cracker, slot[i].ItemPositionX, slot[i].ItemPositionY, WHITE);
						break;
					}
					case 4:
					{
						DrawTexture(KitchenKnife, slot[i].ItemPositionX, slot[i].ItemPositionY, WHITE);
						break;
					}
					case 5:
					{
						DrawTexture(Butter, slot[i].ItemPositionX, slot[i].ItemPositionY, WHITE);
						break;
					}
					case 6:
					{
						DrawTexture(Fork, slot[i].ItemPositionX, slot[i].ItemPositionY, WHITE);
						break;
					}
					case 7:
					{
						DrawTexture(Plate, slot[i].ItemPositionX, slot[i].ItemPositionY, WHITE);
						break;
					}
					case 8:
					{
						DrawTexture(Cup, slot[i].ItemPositionX, slot[i].ItemPositionY, WHITE);
						break;
					}
					case 9:
					{
						DrawTexture(Bowl, slot[i].ItemPositionX, slot[i].ItemPositionY, WHITE);
						break;
					}
					case 10:
					{
						DrawTexture(Granade, slot[i].ItemPositionX, slot[i].ItemPositionY, WHITE);
						break;
					}
				}
				
			}
		}



	}


};

Equipment equipment;

class Door {
public:

	float DoorHeight = 200;
	float DoorWidth = 80;
	const int DoorAmount = 40;
	float DoorPosX[40] =     { 750, 1250, 5000, 10000, 1990, 12950, 15000, 20300,5300,23500,24000, 25750,24250, 27250, 24600,  29250, 29400, 31300, 4750,33250,34250,34750,34990,39250,37300,36250,41500,5400, 4500, 46000,47000,50500, 48300, 53050 };
	float DoorPosY[40] =     { 300, 300, 300,   300,   300,   220,  220,  300,    300,  300, 300,   300  ,300,    300,   300,   300,   300,   300, 300, 300,   300,   300 ,300,  300,   300, 300   , 300,  300, 300, 300,  300,    300,   300, 300 };
	int DoorID[40] =         { 1,    2,   3,     4,    5,       6,   7,    8,      9,   10,   11,   12,    13,     14,    15,    16,   17,    18,   19, 20,    21,   22,    23,    24,   26, 25    , 27 ,  28 , 29,   30,   31,    32,   33,   34};
	int DoorTargetID[40] =   { 3,    4,   1,     2,    6,       5,   8,    7,      10,  9,    12,   11,    14,     13,    16,    15,   18,    17,   20, 19,    25,   26,    24,    23,   22, 21    , 28,   27  ,30,   29,   32,     31,   34,   33,}; 
	int floor[40] =          { 0,    0,   0,     0,   -2,       0,  -2,   -2,      1,   0,     1,    1,     1,      1,     1,     1,    1,     1,   2,  0,     2,    2,     2,     2,     2,  2    , 0,    3,    -1,   0,    -1,      -1,    -1,    -1 };
	int room[40] =           { 0,    1,   2,     2,    3,       2,   4,    3,      5,   0,     6,    5,     7,      5,     8,     5,    9,     8,   10, 0,     11,   12,     13,   10,   10, 10    , 0,   14,   15,   0,    16,     15,   17,   15};




	void DoorEntrance()
	{
		int ArrayFallow = 0;
		int EnteredID = 0;
		int TargetID = 0;
		bool canEnterDoor = false;
		for (int i = 0; i < 50; i++)
		{
			//printf("DoorEntrance Opened\n");
			//printf("Player position X %lf\n", player.PlayerPosX);
			//printf("Player Y %lf\n", player.PlayerPosY);
			if (player.PlayerPosX > DoorPosX[i] && player.PlayerPosX < DoorPosX[i] + DoorWidth)
			{
				if (player.PlayerPosY > DoorPosY[i] && player.PlayerPosY < DoorPosY[i] + DoorHeight)
				{

					canEnterDoor = true;


					EnteredID = DoorID[i];
					ArrayFallow = i;
					//printf("Door Checkpoint passed. Entered Door ID: %d Array Pos: %d\n", EnteredID, ArrayFallow);
				}



			}



		}


		if (canEnterDoor)
		{
			TargetID = DoorTargetID[ArrayFallow];

			equipment.room = room[ArrayFallow];
			equipment.floor = floor[ArrayFallow];

			for (int i = 0; i < DoorAmount; i++)
			{
				if (TargetID == DoorID[i])
				{
					ArrayFallow = i;
					break;

				}
			}


			//printf("Player Pos should change to %d, ArrayFallow: %d\n", DoorTargetID[ArrayFallow], ArrayFallow);
			player.PlayerPosX = DoorPosX[ArrayFallow];
			player.PlayerPosY = DoorPosY[ArrayFallow];

		}
	}



	void DoorDraw()
	{
		for (int i = 0; i < DoorAmount; i++)
		{
			//printf("Door Drawn");
			DrawRectangleRec(Rectangle{DoorPosX[i], DoorPosY[i], DoorWidth, DoorHeight}, YELLOW);
		}
	}


};

Door door;

class Background {
public:

	float CloudPositions[10] = {12000, -400,14000,-400,15000,-500, 18000 , -450, 11000, -600};

	void CloudsRender()
	{
		CloudPositions[0] = CloudPositions[0] + 5;
		CloudPositions[2] = CloudPositions[2] + 3;
		CloudPositions[4] = CloudPositions[4] + 4;
		CloudPositions[6] = CloudPositions[4] + 5;
		CloudPositions[8] = CloudPositions[4] + 6;

		for (int i = 0; i < 2; i++)
		{
			if (CloudPositions[i * 2] > 19000)
			{
				CloudPositions[i * 2] = 10000;
			}
		}
		


		DrawRectangle(11000, -500, 9000, 900, SKYBLUE);
		if (equipment.room == 3) 
		{ 
			DrawTexture(Sun, 18000 * pow((player.PlayerPosX / 18000), 0.9), -300, { 250, 250, 250, 100 }); 
			DrawTexture(BackgroundTexture, 16000 * pow((player.PlayerPosX / 16000), 0.8), player.PlayerPosY * pow(player.PlayerPosY/400, 0.9), { 250, 250, 250, 100 });
		}
		DrawTexture(Cloud3, CloudPositions[0],CloudPositions[1], WHITE);
		DrawTexture(Cloud2, CloudPositions[2], CloudPositions[3], RAYWHITE);
		DrawTexture(Cloud3, CloudPositions[4], CloudPositions[5], WHITE);
		DrawTexture(Cloud2, CloudPositions[6], CloudPositions[7], WHITE);
		DrawTexture(Cloud3, CloudPositions[8], CloudPositions[9], WHITE);
		
		
	}

};

Background background;

class Entity {
public:
	class Bird {
	public:
		int birdAmount = 3;
		struct BirdPerformance
		{
			int ID;
			Vector2 Position;
			Vector2 Speed;
			int mass;
			Color color;
			int maxAlt;
			int	minAlt;
			int accel;
			int drag;
			int maxSpd;
			int minSpd;

			bool goUp;
			bool Boost;


		};

		BirdPerformance birdstats[3]{
			{0, {1, 1},{1, 1}, 10, RED, -200, 200, 10, 1, 20, 10, false, false},
			{0, {1, 1},{1, 1}, 10, RED, -200, 200, 10, 1, 20, 10, false, false},
			{0, {1, 1},{1, 1}, 10, RED, -200, 200, 10, 1, 20, 10, false, false},

		};

		void BirdUpdate()
		{
			int mediumAlt;
			int boostChance;
			for (int i = 0; i < birdAmount; i++)
			{

				//Boost chance determination
				mediumAlt = birdstats[i].maxAlt + birdstats[i].minAlt;

				if (birdstats[i].Position.y < birdstats[i].maxAlt)
				{
					birdstats[i].goUp = true;
				}

				if (birdstats[i].Speed.x < birdstats[i].minSpd)
				{
					birdstats[i].Boost = true;
				}
				if (birdstats[i].Speed.x < birdstats[i].maxSpd)
				{
					birdstats[i].Boost = false;
				}

				
				//Bird Physics 

				birdstats[i].Position.y = birdstats[i].Position.y - birdstats[i].Speed.y;

				if (birdstats[i].goUp)
				{	
					birdstats[i].Speed.y = birdstats[i].Speed.y + (birdstats[i].Speed.y + birdstats[i].accel/5);
				}
				birdstats[i].Speed.y = birdstats[i].Speed.y - birdstats[i].Speed.y * birdstats[i].drag;

				birdstats[i].Position.x = birdstats[i].Position.x + birdstats[i].Speed.x;

				if (birdstats[i].Boost)
				{
					birdstats[i].Speed.x = birdstats[i].Speed.x + (birdstats[i].Speed.x + birdstats[i].accel);
				}
				birdstats[i].Speed.y = birdstats[i].Speed.y - birdstats[i].Speed.y * birdstats[i].drag;


			}






		}

		void BirdRenders()
		{
			for (int i = 0; i < birdAmount; i++)
			{
				DrawRectangle(birdstats[i].Position.x, birdstats[i].Position.y, 30, 30, birdstats[i].color);
			}
		}

	};

	Bird bird;

	class Rat {
	public:

		float Health = 1;
		float PosX = 0;
		float PosX1 = 1500;
		float PosX2 = 0;
		float PosY = 300;
		float PosY1 = 300;
		float PosY2 = 300;
		float timer1 = 3;
		float* timer1pointer = &timer1;

		int Speed = 5;

		bool alive = true;

		

		int MoveRight = 0;


		void VaccumUpdate()
		{
			if (Health <= 0)
			{
				alive = false;
			}
			if (alive)
			{
			*timer1pointer = *timer1pointer - GetFrameTime();
			//Timer Handling
			if (*timer1pointer < 0)
			{

			//	printf("Timer Mice Run Out");
				std::default_random_engine generator1(std::chrono::steady_clock::now().time_since_epoch().count());
				std::uniform_int_distribution<int> distribution1(1, 2);
				*timer1pointer = distribution1(generator1);

				Speed = distribution1(generator1);

				Speed = Speed - 1;
				Speed = Speed * 5;

				MoveRight = distribution1(generator1);

			//	printf("MR %d, %d", MoveRight, Speed);
				
			}


				if (MoveRight == 1)
				{
					PosX = PosX + Speed;

					if (PosX > PosX1)
					{
						MoveRight = 0;
					}
				}
				else {
					PosX = PosX - Speed;

					if (PosX < PosX2)
					{
						MoveRight = 1;
					}
				}
				//Player Hit
				if (abs(player.PlayerPosX - PosX) < 100)
				{
					if (player.PlayerActionHappend)
					{
						Health = Health - player.Damage;
						player.PlayerActionHappend = false;
					}
				}
				//Player Granade
				if (equipment.ItemStats[equipment.SpecialItemInUse].ItemSpecialBool)
				{
					if (equipment.ItemStats[equipment.SpecialItemInUse].ItemSpecialBool)
					{
						equipment.ExplosiveDamageExplosionDetection(Vector2{PosX, PosY}, Vector2{equipment.ItemPositionX[equipment.SpecialItemInUse],equipment.ItemPositionY[equipment.SpecialItemInUse]}, 1, equipment.explosiveStat[equipment.ItemStats[equipment.SpecialItemInUse].RefferenceID].ItemRange,
							equipment.explosiveStat[equipment.ItemStats[equipment.SpecialItemInUse].RefferenceID].ItemDamage, equipment.explosiveStat[equipment.ItemStats[equipment.SpecialItemInUse].RefferenceID].PressurePowerFactor, 0, 0, &Health);

					}
				}
			}
			else {
				PosY = 350;
			}

		}



		void VaccumRedner()
		{
			if (alive)
			{
				Rectangle RecVaccum = { PosX, PosY, 100, 100 };
				DrawRectangleRec(RecVaccum, BLUE);
				DrawText(TextFormat("%fl", Health), PosX, PosY, 20, BLACK);
			}
			else {
				Rectangle RecVaccum = { PosX, PosY, 100, 100 };
				DrawRectangleRec(RecVaccum, BLUE);
			}
		}

	};

	Rat vaccum;

	class Sore {
	public:

		int NextAction = 2;

		struct SorePerformance
		{
			float Health;
			int Armor;
			int WalkingSpeed;
			int RunningSpeed;
			int State; // 0 - sleep, 1 - on duty, 2 - chasing player
			int FrontSearchAreaOnRun;
			int RearSearchAreaOnRun;
			int FrontSearchAreaSteady;
			int Attack1AreaX;
			int Attack1AreaY;
			int Attack2AreaX;
			int Attack2AreaY;
			int ShockTime;

			bool Alive;
			bool Active;
			bool SeePlayer;
			bool FacingRight;

			Vector2 Position;
			Vector2 Speed;

			float Timer1;
			float Timer2;

			int MaxPos;
			int MinPos;



		};

		SorePerformance sore[2] =
		{
			{100, 5, 5, 20, 0, 600, 200, 200, 100, 50, 50, 50, 3 ,true, false, false, true, { 24000, 300 }, { 20, 0 }, 3 , 3 , 25000, 23000},
			{100, 5, 5, 20, 0, 600, 200, 200, 100, 50, 50, 50, 3 ,true, false, false, true, {24000, 300 }, { 20, 0 }, 3 , 3 , 25000, 23000}
		};

		void SoreState0()
		{
			bool Playerdetected = false;

			//Entity patrols area from one side to another. 
			sore[1].Timer1 = sore[1].Timer1 - GetFrameTime(); //Timer countdown

			//Timer1 Handles Random movement
			if (sore[1].Timer1 < 0)
			{
				std::default_random_engine generator1(std::chrono::steady_clock::now().time_since_epoch().count());
				std::uniform_int_distribution<int> distribution1(3, 7);
				sore[1].Timer1 = (float)distribution1(generator1);


				//Next Action Generation
				std::default_random_engine generator2(std::chrono::steady_clock::now().time_since_epoch().count());
				std::uniform_int_distribution<int> distribution2(1, 4);
				NextAction = distribution2(generator2);
			}
			if (sore[1].Position.x > sore[1].MaxPos)
			{
				NextAction = 1;
			}
			else if (sore[1].Position.x < sore[1].MinPos)
			{
				NextAction = 4;
			}




				switch (NextAction)
				{
					case(1):
					{
						sore[1].FacingRight = false;
						sore[1].Speed.x = -sore[1].WalkingSpeed;
						break;
					}
					case(2):
					{
						sore[1].FacingRight = false;
						sore[1].Speed.x = 0;
						break;
					}
					case(3):
					{
						sore[1].FacingRight = true;
						sore[1].Speed.x = 0;
						break;
					}
					case(4):
					{
						sore[1].FacingRight = true;
						sore[1].Speed.x = sore[1].WalkingSpeed;
						break;
					}
				}

				//printf("\nPlayer Pos: %f \n SorePos: %f \n SoreState: %d\n Next Action: %d\n", player.PlayerPosX, sore[1].Position.x, sore[1].State, NextAction, sore[1].Speed.x);


			//Movement Handling
			sore[1].Position.x = sore[1].Position.x + sore[1].Speed.x;


			//Player Detection
			if (abs(player.PlayerVelX) > 2) //Loud Mode
			{
					if ((sore[1].Position.x + sore[1].RearSearchAreaOnRun > player.PlayerPosX) && (player.PlayerPosX > sore[1].Position.x - sore[1].FrontSearchAreaOnRun) && !sore[1].FacingRight)
					{
						Playerdetected = true;
					}
					if ((sore[1].Position.x - sore[1].RearSearchAreaOnRun < player.PlayerPosX) && (player.PlayerPosX < sore[1].Position.x + sore[1].FrontSearchAreaOnRun) && sore[1].FacingRight)
					{
						Playerdetected = true;
					}
			}

			if (abs(player.PlayerVelX) < 2) //Silent Mode
			{
				if ((sore[1].Position.x > player.PlayerPosX) && (player.PlayerPosX > sore[1].Position.x - sore[1].FrontSearchAreaSteady) && !sore[1].FacingRight)
				{
					Playerdetected = true;
				}
				if ((sore[1].Position.x < player.PlayerPosX) && (player.PlayerPosX < sore[1].Position.x + sore[1].FrontSearchAreaSteady) && sore[1].FacingRight)
				{
					Playerdetected = true;
				}
			}

			if (Playerdetected)
			{
				sore[1].State = 1;
				//printf("Playerdetected");
				Playerdetected = false;
			}

			


		}
		void SoreState1()
		{
			bool Playerdetected = false;

			//Entity patrols area from one side to another. 
			sore[1].Timer1 = sore[1].Timer1 - GetFrameTime(); //Timer countdown

			//Timer1 Handles Random movement
			if (sore[1].Timer1 < 0)
			{
				std::default_random_engine generator1(std::chrono::steady_clock::now().time_since_epoch().count());
				std::uniform_int_distribution<int> distribution1(3, 7);
				sore[1].Timer1 = (float)distribution1(generator1);


				//Next Action Generation
				std::default_random_engine generator2(std::chrono::steady_clock::now().time_since_epoch().count());
				std::uniform_int_distribution<int> distribution2(1, 4);
				NextAction = distribution2(generator2);
			}
			if (sore[1].Position.x > sore[1].MaxPos)
			{
				NextAction = 1;
			}
			else if (sore[1].Position.x < sore[1].MinPos)
			{
				NextAction = 4;
			}




			switch (NextAction)
			{
			case(1):
			{
				sore[1].FacingRight = false;
				sore[1].Speed.x = -sore[1].WalkingSpeed;
				break;
			}
			case(2):
			{
				sore[1].FacingRight = false;
				sore[1].Speed.x = 0;
				break;
			}
			case(3):
			{
				sore[1].FacingRight = true;
				sore[1].Speed.x = 0;
				break;
			}
			case(4):
			{
				sore[1].FacingRight = true;
				sore[1].Speed.x = sore[1].WalkingSpeed;
				break;
			}
			}

			//printf("\nPlayer Pos: %f \n SorePos: %f \n SoreState: %d\n Next Action: %d\n", player.PlayerPosX, sore[1].Position.x, sore[1].State, NextAction, sore[1].Speed.x);


		//Movement Handling
			sore[1].Position.x = sore[1].Position.x + sore[1].Speed.x;


			//Player Detection
			if (abs(player.PlayerVelX) > 2) //Loud Mode
			{
				if (abs(player.PlayerVelX) < 2) //Silent Mode
				{
					if ((sore[1].Position.x + 100 + sore[1].RearSearchAreaOnRun > player.PlayerPosX) && (player.PlayerPosX > sore[1].Position.x - sore[1].FrontSearchAreaOnRun + 100) && !sore[1].FacingRight)
					{
						Playerdetected = true;
					}
					if ((sore[1].Position.x + 100 - sore[1].RearSearchAreaOnRun < player.PlayerPosX) && (player.PlayerPosX < sore[1].Position.x + sore[1].FrontSearchAreaOnRun + 100) && sore[1].FacingRight)
					{
						Playerdetected = true;
					}
				}
			}

			if (abs(player.PlayerVelX) < 2) //Silent Mode
			{
				if ((sore[1].Position.x + 100 > player.PlayerPosX) && (player.PlayerPosX > sore[1].Position.x - sore[1].FrontSearchAreaSteady + 100) && !sore[1].FacingRight)
				{
					Playerdetected = true;
				}
				if ((sore[1].Position.x + 100 < player.PlayerPosX) && (player.PlayerPosX < sore[1].Position.x + sore[1].FrontSearchAreaSteady + 100) && sore[1].FacingRight)
				{
					Playerdetected = true;
				}
			}

			if (Playerdetected)
			{
				sore[1].State = 2;
			}
			else {
				sore[1].State = 1;
			}
		}
		void SoreState2()
		{

			if (player.PlayerPosX > sore[1].Position.x)
			{
				sore[1].Speed.x = sore[1].RunningSpeed;
			}
			else
			{
				sore[1].Speed.x = -sore[1].RunningSpeed;
			}

			sore[1].Position.x = sore[1].Position.x + sore[1].Speed.x;

			if (abs(player.PlayerPosX - sore[1].Position.x + 100) < 2000)
			{
				sore[1].State = 2;
				//printf("Player chased");
			}
			else {
				sore[1].State = 1;
			}


			if ((abs(player.PlayerPosX - sore[1].Position.x) < 100) && abs(player.PlayerPosY - sore[1].Position.y) < 100)
			{
				sore[1].State = 1;
				player.IsPlayerDead = true;
				equipment.room = 3;
				equipment.floor = -2;
			}


		}
		void SoreUpdate()
		{
			if (sore[1].Alive)
			{
				switch (sore[1].State)
				{
				case(0):
				{
					SoreState0();
					break;
				}
				case(1):
				{
					SoreState1();
					break;
				}
				case(2):
				{
					SoreState2();
					break;
				}

				}

				if (abs(player.PlayerPosX - sore[1].Position.x) < player.PlayerAttackRange) //Player Attack
				{
					if (player.PlayerActionHappend)
					{
						float ResultingDamage = 0;
						ResultingDamage = player.Damage - sore[1].Armor;
						if (ResultingDamage > player.Damage)
						{
							ResultingDamage = player.Damage;
						}

						sore[1].Health = sore[1].Health - ResultingDamage;
						player.PlayerActionHappend = false;
					}
				}

				//Granade Damage
				if (equipment.ItemStats[equipment.SpecialItemInUse].ItemSpecialBool)
				{
						equipment.ExplosiveDamageExplosionDetection(sore[1].Position,Vector2 { equipment.ItemPositionX[equipment.SpecialItemInUse],equipment.ItemPositionY[equipment.SpecialItemInUse]},sore[1].Armor, equipment.explosiveStat[equipment.ItemStats[equipment.SpecialItemInUse].RefferenceID].ItemRange,
equipment.explosiveStat[equipment.ItemStats[equipment.SpecialItemInUse].RefferenceID].ItemDamage, equipment.explosiveStat[equipment.ItemStats[equipment.SpecialItemInUse].RefferenceID].PressurePowerFactor, 0, 0, &sore[1].Health);
					
				}

				if (sore[1].Health < 0)
				{
					sore[1].Alive = false;
				}
			}
			else if (!sore[1].Alive)
			{
				//Nothing
			}
		
		}

		void SoreRender()
		{
			if (sore[1].Alive)
			{
				Rectangle SoreRec{ sore[1].Position.x - 100, sore[1].Position.y - 100, 200, 200 };
				DrawRectangleRec(SoreRec, GRAY);
				DrawCircle(sore[1].Position.x, sore[1].Position.y, 10, RED);
			}
			else {
				Rectangle SoreRec{ sore[1].Position.x - 100, sore[1].Position.y + 100, 200, 200 };
				DrawRectangleRec(SoreRec, GRAY);
				DrawCircle(sore[1].Position.x, sore[1].Position.y, 10, RED);

			}
		}

	};

	Sore soreEnemy;

};

Entity entity;



typedef struct Room {
	Rectangle rect;
	int ID;
	int blocking;
	bool collision;
	Color color;
} EnvItem;

Room room[] = {
{{0, 100, 2000, 300 },2, 0, false, BROWN }, // level 0
{{4500, -200, 1000, 600 }, 0, 0, false, BROWN },
{{9500, 100, 1000, 300 }, 1, 0, false, BROWN },
{{13000, -8500, 5000, 9000 }, 3, 0, false, SKYBLUE }, // level -2
{{20000, 100, 500, 300 }, 4, 0, false, DARKBROWN },
{{23000, 100, 2000, 300 }, 5, 0, false, DARKGREEN }, // level 1
{{25500, 100, 500, 300 }, 6, 0, false, DARKGREEN },
{{27000, 100, 500, 300 }, 7, 0, false, DARKGREEN },
{{29000, 100, 500, 300 }, 8, 0, false, DARKGREEN },
{{31000, 100, 750, 300 }, 9, 0, false, DARKGREEN },
{{33000, 100, 2000, 300 }, 10, 0, false, LIME }, // level 2
{{36000, 100, 500, 300 }, 11, 0, false, LIME },
{{37000, 100, 600, 300 }, 12, 0, false, LIME },
{{39000, 100, 750, 300 }, 13, 0, false, LIME },
{{41000, 100, 3000, 300 }, 14, 0, false, DARKBROWN }, // level 3
{{46000, 100, 2500, 300 }, 15, 0, false, DARKGRAY }, // level -1
{{50000, 100, 1100, 300 }, 16, 0, false, DARKGRAY },
{{53000, 100, 200, 300 }, 17, 0, false, DARKGRAY },
};



Rectangle TextureRectA{ 0, 0, 9999,9999 };
Rectangle TextureRectB{ 0, 0, 9999,9999 };

void Collisions()
{
	int CurRoom = 0;
	for (int i = 0; i < 100; i++)
	{
		if (equipment.room == room[i].ID)
		{
			CurRoom = i;
			break;
		}
	}

	if (player.PlayerPosX < room[CurRoom].rect.x)
	{
		player.LeftCollision = true;
		if (player.PlayerVelX < 0)
		{
			player.PlayerVelX = 0;
		}
	}
	else { player.LeftCollision = false; }

	if (player.PlayerPosX > room[CurRoom].rect.x + room[CurRoom].rect.width)
	{
		player.RightCollision = true;
		if (player.PlayerVelX > 0)
		{
			player.PlayerVelX = 0;
		}
	}
	else { player.RightCollision = false; }

	if (player.RightItemCollision)
	{
		if (player.PlayerVelX > 0)
		{
			player.PlayerVelX = 0;
		}
	}

	if (player.LeftItemCollision)
	{
		if (player.PlayerVelX < 0)
		{
			player.PlayerVelX = 0;
		}
	}

}

void DrawBackgrounds()
{
	DrawTexturePro(Room1Wall, TextureRectA, room[0].rect, Vector2{ 0 , 0 }, 0, WHITE);
	DrawTexturePro(Room1Wall, TextureRectA, room[1].rect, Vector2{ 0 , 0 }, 0, WHITE);
	DrawTexturePro(Room1Wall, TextureRectA, room[2].rect, Vector2{ 0 , 0 }, 0, WHITE);

	DrawTexturePro(Room2Wall, TextureRectA, room[5].rect, Vector2{ 0 , 0 }, 0, WHITE);
	DrawTexturePro(Room2Wall, TextureRectA, room[6].rect, Vector2{ 0 , 0 }, 0, WHITE);
	DrawTexturePro(Room2Wall, TextureRectA, room[7].rect, Vector2{ 0 , 0 }, 0, WHITE);
	DrawTexturePro(Room2Wall, TextureRectA, room[8].rect, Vector2{ 0 , 0 }, 0, WHITE);

	DrawTexturePro(Room2Wall, TextureRectB, room[9].rect, Vector2{ 0 , 0 }, 0, WHITE);

	DrawTexturePro(Room3Wall, TextureRectB, room[10].rect, Vector2{ 0 , 0 }, 0, WHITE);
	DrawTexturePro(Room3Wall, TextureRectB, room[11].rect, Vector2{ 0 , 0 }, 0, WHITE);
	DrawTexturePro(Room3Wall, TextureRectB, room[12].rect, Vector2{ 0 , 0 }, 0, WHITE);

	DrawTexturePro(Room3Wall, TextureRectA, room[13].rect, Vector2{ 0 , 0 }, 0, WHITE);

	DrawTexturePro(Room4Wall, TextureRectB, room[14].rect, Vector2{ 0 , 0 }, 0, WHITE);

	DrawTexturePro(Room5Wall, TextureRectB, room[15].rect, Vector2{ 0 , 0 }, 0, WHITE);
	DrawTexturePro(Room5Wall, TextureRectB, room[16].rect, Vector2{ 0 , 0 }, 0, WHITE);
	DrawTexturePro(Room5Wall, TextureRectB, room[17].rect, Vector2{ 0 , 0 }, 0, WHITE);



	DrawTexture(GrassTexture, 13000, 190, WHITE);
	DrawTexture(GrassTexture, 13700, 190, WHITE);
	DrawTexture(GrassTexture, 14400, 190, WHITE);
	DrawTexture(GrassTexture, 15100, 190, WHITE);
	DrawTexture(GrassTexture, 15800, 190, WHITE);
	DrawTexture(GrassTexture, 16500, 190, WHITE);
	DrawTexture(GrassTexture, 17200, 190, WHITE);
	DrawTexture(GrassTexture, 17800, 190, WHITE);
	DrawTexture(GrassTexture, 18300, 190, WHITE);
	DrawTexture(GrassTexture, 19000, 190, WHITE);

	DrawTexture(ShedTexture, 14850, 80, WHITE);
	DrawTexture(TrashcanTexture3, 13800, 270, WHITE);
	DrawTexture(TrashcanTexture3, 13900, 270, WHITE);

	DrawTexture(LampTexture2, 100, 110, WHITE);
	DrawTexture(WardobleTexture1, 900, 40, WHITE);
	DrawTexture(TrashcanTexture1, 1900, 300, WHITE);


}

void DrawItemTextures()
{
	
}

int main() {




	InitWindow(screenWidth, screenHeight, "LobsterLobsterLobsterLobsterLobsterLobsterLobsterLobsterLobster");

	SetTargetFPS(40);

	BeginDrawing();
	ClearBackground(BLACK);
	DrawText("Loading", screenWidth/2 - 100, screenHeight/2, 20, LIGHTGRAY);




	LobsterRight = LoadTexture("Graphics/LobsterRight.png");
	LobsterLeft = LoadTexture("Graphics/LobsterLeft.png");
	Room1Wall = LoadTexture("Graphics/Room1WallTexture.png");
	Room2Wall = LoadTexture("Graphics/Room2Wall.png");
	Room3Wall = LoadTexture("Graphics/Room3Wall.png");
	Room4Wall = LoadTexture("Graphics/Room4Wall.png");
	Room5Wall = LoadTexture("Graphics/Room5Wall.png");
	itemsTexture = LoadTexture("Graphics/itemTexture.png");
	Cloud1 = LoadTexture("Graphics/Cloud1.png");
	Cloud2 = LoadTexture("Graphics/Cloud2.png");
	Cloud3 = LoadTexture("Graphics/Cloud3.png");
	Sun = LoadTexture("Graphics/Sun.png");
	BackgroundTexture = LoadTexture("Graphics/Background.png");


	Bowl = LoadTexture("Graphics/Bowl.png");
	Butter = LoadTexture("Graphics/Butter.png");
	Cracker = LoadTexture("Graphics/Cracker.png");
	Cup = LoadTexture("Graphics/Cup.png");
	Fork = LoadTexture("Graphics/Fork.png");
	Granade = LoadTexture("Graphics/Granade.png");
	KitchenKnife = LoadTexture("Graphics/KitchenKnife.png");
	Pen = LoadTexture("Graphics/Pen.png");
	Plate = LoadTexture("Graphics/Plate.png");
	Stone = LoadTexture("Graphics/Stone.png");
	Picture = LoadTexture("Graphics/Picture.png");
	ShedTexture = LoadTexture("Graphics/ShedTexture.png");
	WardobleTexture1 = LoadTexture("Graphics/Wardoble1.png");
	KitchenTexture = LoadTexture("Graphics/KitchenTexture.png");
	LampTexture1 = LoadTexture("Graphics/Lamp1.png");
	LampTexture2 = LoadTexture("Graphics/Lamp2.png");
	GrassTexture = LoadTexture("Graphics/GrassTexture.png");
	TrashcanTexture2 = LoadTexture("Graphics/Trashcan.png");
	TrashcanTexture1 = LoadTexture("Graphics/TrashcanTexture1.png");
	TrashcanTexture3 = LoadTexture("Graphics/TrashcanTexture3.png");
	DeskTexture1 = LoadTexture("Graphics/DeskStand.png");
	DeskTexture2 = LoadTexture("Graphics/DeskTexture.png");
	//GrassTexture = LoadTexture("Graphics/GrassTexture.png");

	InitAudioDevice();

	Sound Music1;
	Sound Music2;
	Sound Walking;
	Sound HitKnife;
	Sound Throw;
	Sound Hit_Claw;
	Sound Frag1;
	Sound Frag2;
	Sound EnemyWalking;

 Walking = LoadSound("Graphics/Walking.wav");
 HitKnife = LoadSound("Graphics/HitKnife.wav");
 Throw = LoadSound("Graphics/Throw.wav");
 Hit_Claw = LoadSound("Graphics/HitClaw.wav");
 Frag1 = LoadSound("Graphics/Frag1.wav");
 Frag2 = LoadSound("Graphics/Frag2.wav");
 EnemyWalking = LoadSound("Graphics/EnemyWalking.wav");

	
	SetMasterVolume(50);



	int envItemsLength = sizeof(room) / sizeof(room[0]);

	Camera2D camera = { 0 };
	camera.target = { player.PlayerPosX, player.PlayerPosY };
	camera.offset = { screenWidth / 2.0f, screenHeight / 2.0f };
	camera.rotation = 0.0f;
	camera.zoom = 1.0f;




	int Tmptimer = 30;
	EndDrawing();
	while (!WindowShouldClose() && !menu.GameExit && IsAudioDeviceReady())
	{
		if (menu.GameState == 1)
		{
			camera.target = { player.PlayerPosX, player.PlayerPosY };

			if (menu.resp)
			{
				menu.resp = false;
				player.PlayerRespawn();
				equipment.room = 3;
				equipment.floor = -2;
			}


			//Game Update

			player.PlayerMovement();
			player.PlayerUpdate();
			equipment.ItemUpdate();
			entity.bird.BirdUpdate();
			entity.soreEnemy.SoreUpdate();
			
			Collisions();

			player.PlayerCollision();

				
			if (IsKeyPressed(KEY_B))
			{
				PlaySound(Hit_Claw);
				player.PlayerAction();
			}

			if (IsKeyDown(KEY_E))
			{
				door.DoorEntrance();
			}
				equipment.ItemUse();
			if (IsKeyDown(KEY_V))
			{
				equipment.ItemPickup();
			}

			if (IsKeyDown(KEY_N))
			{
				equipment.ItemDrop(1);
			}

			if (IsKeyDown(KEY_M))
			{
				equipment.ItemDrop(2);
			}

			entity.vaccum.VaccumUpdate();



		}

		if (IsKeyPressed(KEY_SPACE))
		{
			if (menu.GameState == 1)
			{
				menu.GameState = 0;
			}
			else if (menu.GameState == 0) {
				menu.GameState = 1;
			}
			
		}


		//Drawing

		if (abs(player.PlayerVelX) > 0.1)
		{
			if (Tmptimer < 0)
			{
				PlaySound(Walking);
				//printf("Walking");
				Tmptimer = 30;
			}
			Tmptimer--;
		}
		else { StopSound(Walking); }
		BeginDrawing();
		ClearBackground(BLACK);

		BeginMode2D(camera);
		background.CloudsRender();
		DrawBackgrounds();
		door.DoorDraw();
		entity.vaccum.VaccumRedner();
		entity.bird.BirdRenders();
		
		player.PlayerRender();
		

		equipment.ItemRender();
		DrawItemTextures();
		entity.soreEnemy.SoreRender();

		if (player.PlayerActionRenderRequired)
		{
			player.PlayerActionRender();
		}

		if (equipment.ExploRender)
		{
			equipment.ExplosionRender();
		}
		
		EndMode2D();
		DrawFPS(25, 25);
		equipment.DrawEquipment();
		if (menu.GameState == 0)
		{
			DrawText("Paused", screenWidth / 2, screenHeight / 2, 55, GREEN);
		}
		if (menu.GameState == 2)
		{
			BeginMode2D(camera);
			ClearBackground(BLANK);
			EndMode2D();
			menu.StartMenu();
		}

		if (menu.GameState == 3)
		{
			menu.GameOver();
			if (IsKeyPressed(KEY_SPACE))
			{
				player.PlayerRespawn();
			}
		}
		

		EndDrawing();

	}
	CloseAudioDevice();

	CloseWindow();
	return 0;
}
