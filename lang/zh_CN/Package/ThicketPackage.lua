-- translation for Thicket Package

return {
	["thicket"] = "林包",

	["#caopi"] = "霸业的继承者",
	["caopi"] = "曹丕",
	["illustrator:caopi"] = "SoniaTang",
	["xingshang"] = "行殇",
	[":xingshang"] = "其他角色死亡时，你可以获得该角色的牌。",
	["fangzhu"] = "放逐",
	[":fangzhu"] = "每当你受到一次伤害后，你可以令一名其他角色摸X张牌，然后该角色将其武将牌翻面。（X为你已损失的体力值）",
	["fangzhu-invoke"] = "你可以发动“放逐”<br/> <b>操作提示</b>: 选择一名其他角色→点击确定<br/>",
	["songwei"] = "颂威",
	[":songwei"] = "<font color=\"orange\"><b>主公技，</b></font>其他魏势力角色的黑色判定牌生效后，该角色可以令你摸一张牌。",
	["@songwei-to"] = "请选择“颂威”的目标角色",

	["#xuhuang"] = "周亚夫之风",
	["xuhuang"] = "徐晃",
	["illustrator:xuhuang"] = "Tuu.",
	["duanliang"] = "断粮",
	[":duanliang"] = "你可以将一张黑色的基本牌或黑色的装备牌当【兵粮寸断】使用。<font color=\"blue\"><b>锁定技，</b></font>你可以对距离2以内的其他角色使用【兵粮寸断】。",

	["#menghuo"] = "南蛮王",
	["menghuo"] = "孟获",
	["illustrator:menghuo"] = "废柴男",
	["huoshou"] = "祸首",
	[":huoshou"] = "<font color=\"blue\"><b>锁定技，</b></font>【南蛮入侵】对你无效。每当其他角色使用【南蛮入侵】指定目标后，你成为【南蛮入侵】的伤害来源。",
	["zaiqi"] = "再起",
	[":zaiqi"] = "摸牌阶段开始时，若你已受伤，你可以放弃摸牌，改为从牌堆顶亮出X张牌（X为你已损失的体力值），你回复等同于其中<font color=\"red\">♥</font>牌数量的体力，然后将这些<font color=\"red\">♥</font>牌置入弃牌堆，最后获得其余的牌。",
	["#HuoshouTransfer"] = "%from 的“%arg2”被触发，【<font color=\"yellow\"><b>南蛮入侵</b></font>】的伤害来源改为 %from",

	["#zhurong"] = "野性的女王",
	["zhurong"] = "祝融",
	["illustrator:zhurong"] = "废柴男",
	["juxiang"] = "巨象",
	[":juxiang"] = "<font color=\"blue\"><b>锁定技，</b></font>【南蛮入侵】对你无效。其他角色使用的【南蛮入侵】在结算完毕后置入弃牌堆时，你获得之。",
	["lieren"] = "烈刃",
	[":lieren"] = "每当你使用【杀】对目标角色造成一次伤害后，你可以与该角色拼点：若你赢，你获得其一张牌。",

	["#sunjian"] = "武烈帝",
	["sunjian"] = "孙坚",
	["illustrator:sunjian"] = "LiuHeng",
	["yinghun"] = "英魂",
	[":yinghun"] = "准备阶段开始时，若你已受伤，你可以选择一名其他角色并选择一项：1.令其摸一张牌，然后弃置X张牌；2.令其摸X张牌，然后弃置一张牌。（X为你已损失的体力值）",
	["yinghun-invoke"] = "你可以发动“英魂”<br/> <b>操作提示</b>: 选择一名其他角色→点击确定<br/>",
	["yinghun:d1tx"] = "摸一张牌，然后弃置X张牌",
	["yinghun:dxt1"] = "摸X张牌，然后弃置一张牌",

	["#lusu"] = "独断的外交家",
	["lusu"] = "鲁肃",
	["illustrator:lusu"] = "LiuHeng",
	["haoshi"] = "好施",
	[":haoshi"] = "摸牌阶段，你可以额外摸两张牌，然后若你的手牌多于五张，你将一半（向下取整）的手牌交给手牌数最少的一名其他角色。",
	["@haoshi"] = "请选择“好施”的目标，将一半手牌（向下取整）交给该角色",
	["~haoshi"] = "选择需要给出的手牌→选择一名其他角色→点击确定",
	["dimeng"] = "缔盟",
	[":dimeng"] = "<font color=\"green\"><b>出牌阶段限一次，</b></font>你可以选择两名其他角色并弃置X张牌（X为两名目标角色手牌数的差），令这些角色交换手牌。",
	["#Dimeng"] = "%from (原来 %arg 手牌) 与 %to (原来 %arg2 手牌) 交换了手牌",

	["#jiaxu"] = "冷酷的毒士",
	["jiaxu"] = "贾诩",
	["wansha"] = "完杀",
	[":wansha"] = "<font color=\"blue\"><b>锁定技，</b></font>你的回合内，除濒死角色外的其他角色不能使用【桃】。",
	["weimu"] = "帷幕",
	[":weimu"] = "<font color=\"blue\"><b>锁定技，</b></font>你不能被选择为黑色锦囊牌的目标。",
	["luanwu"] = "乱武",
	[":luanwu"] = "<font color=\"red\"><b>限定技，</b></font>出牌阶段，你可以令所有其他角色对距离最近的另一名角色使用一张【杀】，否则该角色失去1点体力。",
	["@chaos"] = "乱武",
	["@luanwu-slash"] = "请使用一张【杀】响应“乱武”",
	["$LuanwuAnimate"] = "image=image/animate/luanwu.png",
	["#WanshaOne"] = "%from 的“%arg”被触发，只能 %from 自救",
	["#WanshaTwo"] = "%from 的“%arg”被触发，只有 %from 和 %to 才能救 %to",

	["#dongzhuo"] = "魔王",
	["dongzhuo"] = "董卓",
	["illustrator:dongzhuo"] = "小冷",
	["jiuchi"] = "酒池",
	[":jiuchi"] = "你可以将一张♠手牌当【酒】使用。",
	["roulin"] = "肉林",
	[":roulin"] = "<font color=\"blue\"><b>锁定技，</b></font>每当你使用【杀】指定一名女性目标角色后，其需依次使用两张【闪】才能抵消；每当你成为女性角色使用【杀】的目标后，你需依次使用两张【闪】才能抵消。",
	["benghuai"] = "崩坏",
	[":benghuai"] = "<font color=\"blue\"><b>锁定技，</b></font>结束阶段开始时，若你不是当前的体力值最小的角色（或之一），你选择一项：1.失去1点体力；2.减1点体力上限。",
	["benghuai:hp"] = "体力",
	["benghuai:maxhp"] = "体力上限",
	["baonue"] = "暴虐",
	[":baonue"] = "<font color=\"orange\"><b>主公技，</b></font>其他群雄角色造成伤害一次后，该角色可以进行一次判定：若判定结果为♠，你回复1点体力。",
	["@baonue-to"] = "请选择“暴虐”的目标角色",
}
