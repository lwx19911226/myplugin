-- translation for Board Game Magazine Package

return {
	["BGM"] = "☆SP",

	["#bgm_zhaoyun"] = "白马先锋",
	["bgm_zhaoyun"] = "☆SP赵云",
	["&bgm_zhaoyun"] = "赵云",
	["cv:bgm_zhaoyun"] = "墨宣砚韵",
	["illustrator:bgm_zhaoyun"] = "Vincent",
	["designer:bgm_zhaoyun"] = "Danny",
	["chongzhen"] = "冲阵",
	[":chongzhen"] = "每当你发动“龙胆”使用或打出一张手牌时，你可以立即获得对方的一张手牌。",
	["$longdan3"] = "赤胆平乱世，龙枪定江山！",
	["$chongzhen"] = "我等化身为龙，贯穿敌阵！",
	["~bgm_zhaoyun"] = "力量不及，请原谅…",

	["#bgm_diaochan"] = "暗黑的傀儡师",
	["bgm_diaochan"] = "☆SP貂蝉",
	["&bgm_diaochan"] = "貂蝉",
	["illustrator:bgm_diaochan"] = "木美人",
	["designer:bgm_diaochan"] = "Danny",
	["cv:bgm_diaochan"] = "蒲小猫",
	["lihun"] = "离魂",
	["LihunGoBack"] = "请交给目标角色 %arg 张牌",
	[":lihun"] = "出牌阶段，你可以弃置一张牌并将你的武将牌翻面，若如此做，指定一名男性角色，获得其所有手牌。出牌阶段结束时，你须为该角色每一点体力分配给其一张牌。每回合限一次。",
	["$lihun1"] = "倾国之舞，离魂天外。",
	["$lihun2"] = "舞眩人眼，乱人心神。",
	["$lihun3"] = "将军，此舞如何？", --吕布
	["$lihun4"] = "太师，别看呆了哦~", --董卓
	["~bgm_diaochan"] = "董贼已除，我又当何如……",

	["#bgm_caoren"] = "险不辞难",
	["bgm_caoren"] = "☆SP曹仁",
	["&bgm_caoren"] = "曹仁",
	["illustrator:bgm_caoren"] = "张帅",
	["designer:bgm_caoren"] = "Danny",
	["kuiwei"] = "溃围",
	[":kuiwei"] = "回合结束阶段开始时，你可以摸X+2张牌：若如此做，将武将牌翻面，且你的下个摸牌阶段开始时，你弃置X张牌。（X为当前场上武器牌的数量）",
	["yanzheng"] = "严整",
	[":yanzheng"] = "若你的手牌数大于你的体力值，你可以将一张装备区的装备牌当【无懈可击】使用。",
	["@kuiwei"] = "溃围",
	["#KuiweiDiscard"] = "%from 的“%arg2”效果被触发，须弃置 %arg 张牌";
	["cv:bgm_caoren"] = "喵小林",
	["$kuiwei"] = "熬过此战，可见胜机！",
	["$yanzheng"] = "整装列阵，不留破绽！",
	["~bgm_caoren"] = "城在人在，城破人亡……",

	["#bgm_pangtong"] = "荆楚之高俊",
	["bgm_pangtong"] = "☆SP庞统",
	["&bgm_pangtong"] = "庞统",
	["illustrator:bgm_pangtong"] = "LiuHeng",
	["designer:bgm_pangtong"] = "Danny",
	["cv:bgm_pangtong"] = "墨宣砚韵",
	["manjuan"] = "漫卷",
	[":manjuan"] = "每当你将获得一张手牌时，你将之置入弃牌堆。若你的回合内你发动“漫卷”，你可以获得弃牌堆中一张与该牌同点数的牌，你以此法获得的牌不能发动“漫卷”。",
	["$ManjuanGot"] = "%from 即将获得 %card 并将该牌置入弃牌堆",
	["zuixiang"] = "醉乡",
	[":zuixiang"] = "<b>限定技</b>，回合开始阶段开始时，你可以展示牌堆顶的3张牌并置于你的武将牌上，你不可使用或打出与该些牌同类的牌，所有同类牌对你无效。之后每个你的回合开始阶段，你须重复展示一次，直至该些牌中任意两张点数相同时，将你武将牌上的全部牌置于你的手上。",
	["#ZuiXiang1"] = "%from 的“%arg2”效果被触发， %to 的卡牌【%arg】对其无效",
	["#ZuiXiang2"] = "%from 的“%arg2”效果被触发，【%arg】对其无效",
	["$ZuixiangGot"] = "%from 获得了全部的“醉乡”牌 %card",
	["@sleep"] = "醉乡",
	["dream"] = "醉乡",
	["$manjuan"] = "漫卷纵酒，白首狂歌。",
	["$zuixiang"] = "懵懵醉乡中，天下心中藏。",
	["$ZuixiangAnimate"] = "懵懵醉乡中，天下心中藏。",
	["~bgm_pangtong"] = "纵有治世才，难遇治世主…",
	
	["#bgm_zhangfei"] = "横矛立马",
	["bgm_zhangfei"] = "☆SP张飞",
	["&bgm_zhangfei"] = "张飞",
	["illustrator:bgm_zhangfei"] = "绿豆粥",
	["cv:bgm_zhangfei"] = "喵小林",
	["designer:bgm_zhangfei"] = "Serendipity",
	["jie"] = "嫉恶",
	[":jie"] = "<b>锁定技</b>，你使用的红色【杀】造成的伤害+1。",
	["#Jie"] = "%from 的【嫉恶】技能被触发，伤害从 %arg 点上升至 %arg2 点",
	["dahe"] = "大喝",
	[":dahe"] = "出牌阶段，你可以与一名其他角色拼点；若你赢，该角色的非红心【闪】无效直到回合结束，你可将该角色拼点的牌交给场上一名体力不多于你的角色。若你没赢，你须展示手牌并选择一张弃置。每阶段限一次。",
	["$dahe"] = "燕人张飞在此！",
	["@dahe-jink"] = "%src 使用了【杀】，请打出一张【闪】（由于 %dest 的技能【%arg】被触发，你本次响应的非红桃【闪】将无效）",
	["dahe:yes"] = "分配对方拼点牌",
	["dahe:no"] = "不分配",
	["#DaheEffect"] = "%from 的技能【%arg2】被触发，%to 使用的 %arg 【闪】无效",
	["$jie1"] = "杂碎，也敢在爷爷面前叫嚣！",
	["$jie2"] = "三姓家奴，吃我一矛！",
	["~bgm_zhangfei"] = "大哥……二哥……",
	
	["#bgm_lvmeng"] = "国士之风",
	["bgm_lvmeng"] = "☆SP吕蒙",
	["&bgm_lvmeng"] = "吕蒙",
	["illustrator:bgm_lvmeng"] = "YellowKiss",
	["designer:bgm_lvmeng"] = "如水法师卞程",
	["cv:bgm_lvmeng"] = "风叹息",
	["tanhu"] = "探虎",	
	[":tanhu"] = "出牌阶段，你可以与一名其他角色拼点。若你赢，你获得以下技能直到回合结束：你与该角色的距离为1.你对该角色使用的非延时类锦囊不能被【无懈可击】抵消，每阶段限一次。",
	["mouduan"] = "谋断",
	[":mouduan"] = "<b>转化技</b>，通常状态下，你拥有标记“武”并拥有技能“激昂”和“谦逊”。当你的手牌数为2张或以下时，你须将你的标记翻面为“文”，将该两项技能转化为“英姿”和“克己”。任一角色的回合开始前，你可弃一张牌将标记翻回。",
	["@mouduan"] = "你可以弃置一张牌将标记翻至“武”朝上（若弃牌后你的手牌数小于或等于2则无事发生）",
	["@wen"] = "文",
	["@wu"] = "武",
	["$tanhu1"] = "不入虎穴，焉得虎子。",
	["$tanhu2"] = "诈以欺敌，袭其空虚！", --赢
	["$tanhu3"] = "反复之人，不可轻信！", --没赢
	["$mouduan"] = "士别三日，当刮目相看！",
	["$jiang3"] = "陈列赫然，兵人练习！",
	["$jiang4"] = "攻守兼备，进退自如！",
	["$yingzi3"] = "秉承遗志，树威立信。",
	["$yingzi4"] = "明正军纪，路无拾遗！",
	["$keji3"] = "利在不战，长计制之。",
	["$keji4"] = "容忍于心，深藏不露。",
	["~bgm_lvmeng"] = "未见吴之天下，怎敢轻生……",
	
	["#bgm_liubei"] = "汉昭烈帝",
	["bgm_liubei"] = "☆SP刘备",
	["&bgm_liubei"] = "刘备", 
	["illustrator:bgm_liubei"] = "FOOL头",
	["designer:bgm_liubei"] = "妄想线条",
	["cv:bgm_liubei"] = "喵小林",
	["zhaolie"] = "昭烈",
	[":zhaolie"] = "摸牌阶段摸牌时，你可以少摸一张牌，指定你攻击范围内的一名其他角色亮出牌堆顶上3张牌，将其中全部的非基本牌和【桃】置于弃牌堆，该角色进行二选一：你对其造成X点伤害，然后他获得这些基本牌；或他依次弃置X张牌，然后你获得这些基本牌。（X为其中非基本牌的数量）。",
	["zhaolie:damage"] = "受伤害拿牌",
	["zhaolie:throw"] = "弃牌",
	["shichou"] = "誓仇",
	[":shichou"] = "<b>主公技</b>，<b>限定技</b>，你的回合开始时，你可指定一名蜀国角色并交给其两张牌。本盘游戏中，每当你受到伤害时，改为该角色替你受到等量的伤害，然后摸等量的牌，直至该角色第一次进入濒死状态。",
	["@chou"] = "仇",
	["@hate"] = "誓",
	["ShichouGive"] = "请给目标 %arg 张牌（包括装备）",
	["#ShichouProtect"] = "%from 的技能【%arg】被触发，%to 代其承受本次伤害",
	["$zhaolie1"] = "不灭东吴，誓不归蜀！",
	["$zhaolie2"] = "汝等务劝，此战势在必行！",
	["$shichou"] = "尔等叛贼，害我兄弟，饶不得汝！",
	["$ShichouAnimate"] = "尔等叛贼，害我兄弟，饶不得汝！",
	["~bgm_liubei"] = "一时不仁，毁己功业，吾悔矣！",
	
	["#bgm_daqiao"] = "韶光易逝",
	["bgm_daqiao"] = "☆SP大乔",
	["&bgm_daqiao"] = "大乔",
	["illustrator:bgm_daqiao"] = "木美人",
	["designer:bgm_daqiao"] = "Ecauchy",
	["cv:bgm_daqiao"] = "蒲小猫",
	["YanxiaoCard"] = "言笑牌",
	["yanxiao"] = "言笑",
	[":yanxiao"] = "出牌阶段，你可以将一张方块牌置于一名角色的判定区内，判定区内有“言笑”牌的角色下个判定阶段开始时，获得其判定区里的所有牌。",
	["$YanxiaoGot"] = "%from 判定阶段开始时，获得其判定区内所有牌：%card",
	["anxian"] = "安娴",
	[":anxian"] = "每当你使用【杀】对目标角色造成伤害时，你可以防止此次伤害，令其弃置一张手牌，然后你摸一张牌；当你成为【杀】的目标时，你可以弃置一张手牌使之无效，然后该【杀】的使用者摸一张牌。",
	["#Anxian"] = "%from 发动了技能【%arg】，防止了此次伤害。",
	["#AnxianAvoid"] = "%to 使用了【%arg】，%from 对其使用的【杀】无效。",
	["@anxian-discard"] = "你可以弃置一张手牌令此【杀】失效",
	["$yanxiao1"] = "言笑之间，忧散愁消。",
	["$yanxiao2"] = "吾夫有忧色，妾当为解之。", --孙策
	["$anxian1"] = "安淑娴静，岂愿伤人~",
	["$anxian2"] = "岂可如此无礼！",
	["~bgm_daqiao"] = "青灯常伴，了此余生…",
	
	["#bgm_ganning"] = "怀铃的乌羽",
	["bgm_ganning"] = "☆SP甘宁",
	["&bgm_ganning"] = "甘宁",
	["illustrator:bgm_ganning"] = "张帅",
	["designer:bgm_ganning"] = "飞雪",
	["cv:bgm_ganning"] = "风叹息",
	["yinling"] = "银铃",
	[":yinling"] = "出牌阶段，你可以弃置一张黑色牌并指定一名其他角色。若如此做，你获得其一张牌并置于你的武将牌上，称为“锦”。（数量最多为四）",
	["brocade"] = "锦",
	["junwei"] = "军威",
	[":junwei"] = "回合结束阶段开始时，你可以将三张“锦”置入弃牌堆。若如此做，你须指定一名角色并令其选择一项：1.亮出一张【闪】，然后由你交给任意一名角色。2.该角色失去1点体力，然后由你选择将其装备区的一张牌移出游戏。在该角色的回合结束后，将以此法移出游戏的装备牌移回原处。",
	["junwei_equip"] = "军威",
	["@junwei-show"] = "请展示一张【闪】",
	["$JunweiGot"] = "%from 的装备牌 %card 被移回装备区",
	["$yinling1"] = "银铃响，锦帆扬！",
	["$yinling2"] = "老子就是银铃锦帆甘兴霸！",
	["$junwei1"] = "别太嚣张了！",
	["$junwei2"] = "这江上，老子说了算！",
	["~bgm_ganning"] = "小的们，点子扎手，扯呼！",
	
	["#bgm_xiahoudun"] = "啖睛的苍狼",
	["bgm_xiahoudun"] = "☆SP夏侯惇",
	["&bgm_xiahoudun"] = "夏侯惇",
	["illustrator:bgm_xiahoudun"] = "XXX",
	["designer:bgm_xiahoudun"] = "舟亢",
	["cv:bgm_xiahoudun"] = "喵小林",
	["fenyong"] = "愤勇",
	[":fenyong"] = "每当你受到一次伤害后，你可以竖置你的体力牌；当你的体力牌为竖置状态时，防止你受到的所有伤害。",
	["#FenyongAvoid"] = "%from 的“%arg”被触发，防止了本次伤害",
	["xuehen"] = "雪恨",
	[":xuehen"] = "每个角色的回合结束阶段开始时，若你的体力牌为竖置状态，你须横置之，然后选择一项：1.弃置当前回合角色X张牌（X为你已损失的体力值）；2.视为对一名任意角色使用一张【杀】。",
	["@fenyong"] = "愤勇",
	["xuehen:discard"] = "弃置当前回合角色X张牌",
	["xuehen:slash"] = "视为使用一张【杀】",
	["$fenyong1"] = "独目苍狼，虽伤亦勇！",
	["$fenyong2"] = "愤勇当先，鬼神难伤！",
	["$xuehen1"] = "汝等凶逆，岂欲望生乎！",
	["$xuehen2"] = "夺目之恨犹在，今必斩汝！",
	["~bgm_xiahoudun"] = "凛然领军出，马革裹尸还……",
	
	--FOR BGMDIY
	
	["BGMDIY"] = "桌游志贴纸",

	["#diy_simazhao"] = "狼子野心",
	["diy_simazhao"] = "司马昭",
	["illustrator:diy_simazhao"] = "YellowKiss",
	["designer:diy_simazhao"] = "尹昭晨",
	["zhaoxin"] = "昭心",
	[":zhaoxin"] = "摸牌阶段结束时，你可以展示所有手牌，若如此做，视为你使用一张【杀】，每阶段限一次。",
	["@zhaoxin"] = "你可以发动“昭心”",
	["~zhaoxin"] = "选择【杀】的目标角色→点击确定",
	["langgu"] = "狼顾",
	[":langgu"] = "每当你受到1点伤害后，你可以进行一次判定，然后你可以打出一张手牌代替此判定牌：若如此做，你观看伤害来源的所有手牌，并弃置其中任意数量的与判定牌花色相同的牌。",
	["@langgu-card"] = "请发动“%dest”来修改 %src 的 %arg 判定",
	["~langgu"] = "选择一张手牌→点击确定",

	["#diy_wangyuanji"] = "文明皇后",
	["diy_wangyuanji"] = "王元姬",
	["illustrator:diy_wangyuanji"] = "YellowKiss",
	["designer:diy_wangyuanji"] = "尹昭晨",
	["fuluan"] = "扶乱",
	[":fuluan"] = "出牌阶段，若你未于此阶段使用过【杀】，你可以弃置三张相同花色的牌并选择攻击范围内的一名其他角色，该角色将武将牌翻面，且此阶段你不可使用【杀】，每阶段限一次。",
	["shude"] = "淑德",
	[":shude"] = "回合结束阶段开始时，你可以将手牌数补至等同于体力上限的张数。",

	["#diy_liuxie"] = "汉献帝",
	["diy_liuxie"] = "刘协",
	["illustrator:diy_liuxie"] = "XXX",
	["designer:diy_liuxie"] = "姚以轩",
	["huangen"] = "皇恩",
	[":huangen"] = "每当一张锦囊牌指定了不少于两名目标时，你可以令成为该牌目标的至多X名角色各摸一张牌，则该锦囊牌对这些角色无效。（X为你当前体力值）",
	["hantong"] = "汉统",
	[":hantong"] = "弃牌阶段，你可以将你弃置的手牌置于武将牌上，称为“诏”。你可以将一张“诏”置入弃牌堆，然后你拥有并发动以下技能之一：“护驾”、“激将”、“救援”、“血裔”，直到当前回合结束。",
	["edict"] = "诏",
	["@huangen-card"] = "你可以发动“皇恩”",
	["~huangen"] = "选择至多X名角色→点击确定（X为你当前体力值）",
	["hantong_acquire"] = "汉统",
	["hantong_acquire:hujia"] = "你可以发动“汉统”并获得技能“护驾”",
	["hantong_acquire:jijiang"] = "你可以发动“汉统”并获得技能“激将”",
	["hantong_acquire:jiuyuan"] = "你可以发动“汉统”并获得技能“救援”",
	["hantong_acquire:xueyi"] = "你可以发动“汉统”并获得技能“血裔”",
	["@hantong-jijiang"] = "请发动“激将”",
	["~jijiang"] = "选择【杀】的目标→点击确定",

	["#diy_gongsunzan"] = "白马将军",
	["diy_gongsunzan"] = "贴纸公孙瓒",
	["&diy_gongsunzan"] = "公孙瓒",
	["illustrator:diy_gongsunzan"] = "XXX",
	["designer:diy_gongsunzan"] = "爱放泡的鱼",
	["diyyicong"] = "义从",
	[":diyyicong"] = "弃牌阶段结束时，你可以将任意数量的牌置于武将牌上，称为“扈”。每有一张“扈”，其他角色计算与你的距离+1。",
	["tuqi"] = "突骑",
	[":tuqi"] = "<b>锁定技，</b>回合开始阶段开始时，若你的武将牌上有“扈”：你计算与其他角色的距离-X，直到回合结束，若X不大于2，你摸一张牌（X为你武将牌上“扈”的数量），然后你将所有“扈”置入弃牌堆。",
	["retinue"] = "扈",
	["@diyyicong"] = "你可以发动“义从”",
	["~diyyicong"] = "选择若干张牌→点击确定",
	["#diyyicong-dist"] = "义从",
	["#tuqi-dist"] = "突骑",
}
