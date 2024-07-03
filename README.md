# 網路程式設計專題
## 題目：心臟病
組員：李名智、鍾佳妘

### Demo影片網址
https://www.youtube.com/watch?v=vabP2qyw6LQ
### 說明作品
運用Client-Server架構來呈現卡牌雙人遊戲心臟病，Server利用隨機數讓卡牌分成隨機的兩堆，Client利用設定鍵盤來實現翻牌以及搶牌的動作，一旦有人的牌全部出完即獲得勝利，利用Thread來讓Client可同時輸入與接收。

本作品架構：1對多  

本作品的功能：不按enter發送字元、分配卡牌、若有誤將卡牌分配給玩家

本作品使用的技術：thread、非攔阻模式、getch()、隨機數的生成
### 作品執行過程說明
1. 先將Server開啟並連接兩個Client

2. 一個Client以鍵盤”o”來翻牌、以“p”來搶牌

3. 另一個以鍵盤”w”來翻牌、以”q”來搶牌

4. 若在錯誤的時間點按搶牌，顯示error並將已出的牌加進該者牌堆內

5. 若在正確的時間點按搶牌，將已出過的牌加進對方牌堆內

6. 若其中一人牌已出完，遊戲就結束，該者獲得勝利

![image](https://github.com/MingChih123/-Slapjack_Game/assets/172878079/48eedc5e-2c21-4884-aa17-307f4fd151f1)
