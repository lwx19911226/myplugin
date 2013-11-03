module("extensions.demo",package.seeall)
extension=sgs.Package("demo")
luacc=sgs.General(extension,"luacc","wei",4,true)
luasmy=sgs.General(extension,"luasmy","wei",3,true)
luaxhd=sgs.General(extension,"luaxhd","wei",4,true)
luazhl=sgs.General(extension,"luazhl","wei",4,true)
luaxch=sgs.General(extension,"luaxch","wei",4,true)
luagj=sgs.General(extension,"luagj","wei",3,true)
luazhj=sgs.General(extension,"luazhj","wei",3,false)
dofile "lua/sgs_ex.lua"
dofile "lua/myplugin_sysfunc.lua"
luajianxiong=sgs.CreateTriggerSkill{
name="luajianxiong",
events={sgs.Damaged},
frequency=sgs.Skill_NotFrequent,
on_trigger=function(self,event,player,data)
local room=player:getRoom()
local selfskname=self:objectName()
local selfplayer=room:findPlayerBySkillName(selfskname)
local defaultReturn=false
    if event==sgs.Damaged then
        local damage=data:toDamage()
        local getCardPlace_mynum_2
        if not player:hasSkill(selfskname) then return defaultReturn end
        if not damage.card then return defaultReturn end
        getCardPlace_mynum_2=room:getCardPlace(damage.card:getEffectiveId())
        if getCardPlace_mynum_2==sgs.Player_PlaceTable then
            if not room:askForSkillInvoke(selfplayer,selfskname,data) then return defaultReturn end
            selfplayer:obtainCard(damage.card,true)
        else
        end
    end
end,
can_trigger=function(self,player)
local room=player:getRoom()
local selfplayer=room:findPlayerBySkillName(self:objectName())
if selfplayer==nil then return false end
return selfplayer:isAlive()
end,
}
luafankui=sgs.CreateTriggerSkill{
name="luafankui",
events={sgs.Damaged},
frequency=sgs.Skill_NotFrequent,
on_trigger=function(self,event,player,data)
local room=player:getRoom()
local selfskname=self:objectName()
local selfplayer=room:findPlayerBySkillName(selfskname)
local defaultReturn=false
    if event==sgs.Damaged then
        local damage=data:toDamage()
        local getCardPlace_mynum_15
        local cardFromHE_mycard_13
        local cardFromHE_mynum_12
        local isNude_mybool_8
        if not player:hasSkill(selfskname) then return defaultReturn end
        if not damage.from then return defaultReturn end
        isNude_mybool_8=damage.from:isNude()
        if isNude_mybool_8 then return false end
        if not room:askForSkillInvoke(selfplayer,selfskname,data) then return defaultReturn end
        cardFromHE_mynum_12=room:askForCardChosen(selfplayer,damage.from,"he",selfskname)
        cardFromHE_mycard_13=sgs.Sanguosha:getCard(cardFromHE_mynum_12)
        getCardPlace_mynum_15=room:getCardPlace(cardFromHE_mycard_13:getEffectiveId())
        if getCardPlace_mynum_15==sgs.Player_PlaceEquip then
            selfplayer:obtainCard(cardFromHE_mycard_13,true)
        else
            selfplayer:obtainCard(cardFromHE_mycard_13,false)
        end
    end
end,
can_trigger=function(self,player)
local room=player:getRoom()
local selfplayer=room:findPlayerBySkillName(self:objectName())
if selfplayer==nil then return false end
return selfplayer:isAlive()
end,
}
luaganglie=sgs.CreateTriggerSkill{
name="luaganglie",
events={sgs.Damaged},
frequency=sgs.Skill_NotFrequent,
on_trigger=function(self,event,player,data)
local room=player:getRoom()
local selfskname=self:objectName()
local selfplayer=room:findPlayerBySkillName(selfskname)
local defaultReturn=false
    if event==sgs.Damaged then
        local damage=data:toDamage()
        local askForDiscard_mybool_28
        local judge_mycard_26
        local judge_mybool_25
        local isAlive_mybool_21
        if not player:hasSkill(selfskname) then return defaultReturn end
        if not damage.from then return defaultReturn end
        isAlive_mybool_21=damage.from:isAlive()
        if not isAlive_mybool_21 then return false end
        if not room:askForSkillInvoke(selfplayer,selfskname,data) then return defaultReturn end
        local judge104=myjudgestruct("(.*):(heart):(.*)",selfplayer,selfskname,false)
        room:judge(judge104)
        judge_mybool_25=judge104:isGood()
        judge_mycard_26=judge104.card
        if judge104:isGood() then
            askForDiscard_mybool_28=room:askForDiscard(damage.from,selfskname,2,2,false,false)
            if askForDiscard_mybool_28 then
            else
                local damage105=sgs.DamageStruct()
                damage105.from=selfplayer
                damage105.to=damage.from
                damage105.card=nil
                damage105.damage=1
                damage105.nature=sgs.DamageStruct_Normal
                room:damage(damage105)
            end
        else
        end
    end
end,
can_trigger=function(self,player)
local room=player:getRoom()
local selfplayer=room:findPlayerBySkillName(self:objectName())
if selfplayer==nil then return false end
return selfplayer:isAlive()
end,
}
luatuxi=sgs.CreateTriggerSkill{
name="luatuxi",
events={sgs.EventPhaseStart},
frequency=sgs.Skill_NotFrequent,
on_trigger=function(self,event,player,data)
local room=player:getRoom()
local selfskname=self:objectName()
local selfplayer=room:findPlayerBySkillName(selfskname)
local defaultReturn=false
    if event==sgs.EventPhaseStart then
        local phase=player:getPhase()
        local askForUseSkillCard_mybool_87
        if phase==sgs.Player_Draw then
            if not player:hasSkill(selfskname) then return defaultReturn end
            if not room:askForSkillInvoke(selfplayer,selfskname,data) then return defaultReturn end
            askForUseSkillCard_mybool_87=room:askForUseCard(selfplayer,"@".."luatuxivs",selfskname)
            if askForUseSkillCard_mybool_87 then
                if true then return true end
            else
            end
        else
        end
    end
end,
can_trigger=function(self,player)
local room=player:getRoom()
local selfplayer=room:findPlayerBySkillName(self:objectName())
if selfplayer==nil then return false end
return selfplayer:isAlive()
end,
}
lualuoyi=sgs.CreateTriggerSkill{
name="lualuoyi",
events={sgs.DrawNCards,sgs.Predamage},
frequency=sgs.Skill_NotFrequent,
on_trigger=function(self,event,player,data)
local room=player:getRoom()
local selfskname=self:objectName()
local selfplayer=room:findPlayerBySkillName(selfskname)
local defaultReturn=false
    if event==sgs.DrawNCards then
        local drawnum=data:toInt()
        if not player:hasSkill(selfskname) then return defaultReturn end
        if not room:askForSkillInvoke(selfplayer,selfskname,data) then return defaultReturn end
        drawnum=drawnum-1
        room:setPlayerFlag(selfplayer,"flagOn_mystrc_34")
        data:setValue(drawnum)
        return false
    end
    if event==sgs.Predamage then
        local damage=data:toDamage()
        local hasFlag_mybool_44
        local or2_mybool_41
        local isKindOf_mybool_39
        local isKindOf_mybool_37
        if not player:hasSkill(selfskname) then return defaultReturn end
        if not damage.card then return defaultReturn end
        isKindOf_mybool_37=damage.card:isKindOf("Slash")
        isKindOf_mybool_39=damage.card:isKindOf("Duel")
        or2_mybool_41=(isKindOf_mybool_37 or isKindOf_mybool_39)
        if or2_mybool_41 then
            hasFlag_mybool_44=selfplayer:hasFlag("flagOn_mystrc_34")
            if not hasFlag_mybool_44 then return false end
            damage.damage=damage.damage+1
        else
        end
        data:setValue(damage)
        return false
    end
end,
can_trigger=function(self,player)
local room=player:getRoom()
local selfplayer=room:findPlayerBySkillName(self:objectName())
if selfplayer==nil then return false end
return selfplayer:isAlive()
end,
}
luatiandu=sgs.CreateTriggerSkill{
name="luatiandu",
events={sgs.FinishJudge},
frequency=sgs.Skill_Frequent,
on_trigger=function(self,event,player,data)
local room=player:getRoom()
local selfskname=self:objectName()
local selfplayer=room:findPlayerBySkillName(selfskname)
local defaultReturn=false
    if event==sgs.FinishJudge then
        local judge=data:toJudge()
        if not player:hasSkill(selfskname) then return defaultReturn end
        if not room:askForSkillInvoke(selfplayer,selfskname,data) then return defaultReturn end
        if not judge.card then return defaultReturn end
        selfplayer:obtainCard(judge.card,true)
    end
end,
can_trigger=function(self,player)
local room=player:getRoom()
local selfplayer=room:findPlayerBySkillName(self:objectName())
if selfplayer==nil then return false end
return selfplayer:isAlive()
end,
}
lualuoshen=sgs.CreateTriggerSkill{
name="lualuoshen",
events={sgs.EventPhaseStart},
frequency=sgs.Skill_Frequent,
on_trigger=function(self,event,player,data)
local room=player:getRoom()
local selfskname=self:objectName()
local selfplayer=room:findPlayerBySkillName(selfskname)
local defaultReturn=false
    if event==sgs.EventPhaseStart then
        local phase=player:getPhase()
        local judge_mycard_56
        local judge_mybool_55
        local repeatx_mynum_53
        if not player:hasSkill(selfskname) then return defaultReturn end
        if phase==sgs.Player_Start then
            for repeatx_mynum_53=1,100,1 do
                local judge106=myjudgestruct("(.*):(spade|club):(.*)",selfplayer,selfskname,true)
                room:judge(judge106)
                judge_mybool_55=judge106:isGood()
                judge_mycard_56=judge106.card
                if judge106:isGood() then
                    selfplayer:obtainCard(judge_mycard_56,true)
                else
                    if true then return false end
                end
            end
        else
        end
    end
end,
can_trigger=function(self,player)
local room=player:getRoom()
local selfplayer=room:findPlayerBySkillName(self:objectName())
if selfplayer==nil then return false end
return selfplayer:isAlive()
end,
}
luatuxivs_card=sgs.CreateSkillCard{
name="luatuxivs",
target_fixed=false,
will_throw=false,
filter=function(self,targets,to_select)
    local selfskname="luatuxivs"
    local isKongcheng_mybool_69
    local theSame_mybool_66
    local lessThan_mybool_63
    local selfcard=self
    local defaultReturn=false
    lessThan_mybool_63=(#targets<2)
    if not lessThan_mybool_63 then return false end
    theSame_mybool_66=(to_select:getSeat()==sgs.Self:getSeat())
    if theSame_mybool_66 then return false end
    isKongcheng_mybool_69=to_select:isKongcheng()
    if not isKongcheng_mybool_69 then return true end
return false end,
feasible=function(self,targets) return (#targets>=2) end,
on_use=function(self,room,source,targets)
    local selfskname="luatuxivs"
    local cardFromH_mycard_80
    local cardFromH_mynum_79
    local cardFromH_mycard_77
    local cardFromH_mynum_76
    local targetPlayer_mysplayer_74
    local targetPlayer_mysplayer_72
    local selfcard=self
    local selfplayer=source
    local defaultReturn
    targetPlayer_mysplayer_72=targets[1]
    targetPlayer_mysplayer_74=targets[2]
    if not targetPlayer_mysplayer_72 then return defaultReturn end
    if targetPlayer_mysplayer_72:isKongcheng() then return defaultReturn end
    cardFromH_mynum_76=room:askForCardChosen(selfplayer,targetPlayer_mysplayer_72,"h",selfskname)
    cardFromH_mycard_77=sgs.Sanguosha:getCard(cardFromH_mynum_76)
    if not targetPlayer_mysplayer_74 then return defaultReturn end
    if targetPlayer_mysplayer_74:isKongcheng() then return defaultReturn end
    cardFromH_mynum_79=room:askForCardChosen(selfplayer,targetPlayer_mysplayer_74,"h",selfskname)
    cardFromH_mycard_80=sgs.Sanguosha:getCard(cardFromH_mynum_79)
    selfplayer:obtainCard(cardFromH_mycard_77,false)
    selfplayer:obtainCard(cardFromH_mycard_80,false)
end,
}
luatuxivs=sgs.CreateViewAsSkill{
name="luatuxivs",
n=0,
view_as=function(self,cards)
if #cards~=0 then return nil end
local a_card=luatuxivs_card:clone()
for var=1,#cards,1 do a_card:addSubcard(cards[var]) end
a_card:setSkillName(self:objectName())
return a_card
end,
enabled_at_response=function(self,player,pattern)
    local selfskname=self:objectName()
    local patternMatchSkillCard_mybool_60
    local defaultReturn=false
    patternMatchSkillCard_mybool_60=(pattern=="@"..selfskname)
    if true then return patternMatchSkillCard_mybool_60 end
return false end,
}
addsk(luajianxiong)
addsk(luafankui)
addsk(luaganglie)
addsk(luatuxi)
addsk(lualuoyi)
addsk(luatiandu)
addsk(lualuoshen)
addsk(luatuxivs)
luacc:addSkill("luajianxiong")
luasmy:addSkill("luafankui")
luaxhd:addSkill("luaganglie")
luazhl:addSkill("luatuxi")
luaxch:addSkill("lualuoyi")
luagj:addSkill("luatiandu")
luazhj:addSkill("lualuoshen")
luazhl:addSkill("luatuxivs")
addexsk(luacc,"liegong")
demo_trans={}
demo_trans["demo"]="DEMO"
addgtrans(demo_trans,"luacc","CC","","胃疼","")
addgtrans(demo_trans,"luasmy","SM","","天命难违","")
addgtrans(demo_trans,"luaxhd","夏侯哥","","","")
addgtrans(demo_trans,"luazhl","辽神","","真没想到","")
addgtrans(demo_trans,"luaxch","褚","虎痴","冷","")
addgtrans(demo_trans,"luagj","郭嘉","","咳","")
addgtrans(demo_trans,"luazhj","甄姬","","","")
addsktrans(demo_trans,"luajianxiong","奸雄","收AOE",{"我护天下人","梦中杀人"})
addsktrans(demo_trans,"luafankui","反馈","卖血拿牌",{"下次注意点","出来混要还"})
addsktrans(demo_trans,"luaganglie","刚烈","卖血刚",{"竟敢桑我"})
addsktrans(demo_trans,"luatuxi","突袭","放弃摸牌，偷牌",{"没想到8","拿来8"})
addsktrans(demo_trans,"lualuoyi","裸衣","破，杀，决斗",{"破"})
addsktrans(demo_trans,"luatiandu","天妒","收判定",{""})
addsktrans(demo_trans,"lualuoshen","洛神","洛牌",{"飘飘兮"})
addsktrans(demo_trans,"luatuxivs","突袭","偷",{""})
sgs.LoadTranslationTable(cmpltrans(demo_trans))
