function mymatchone(cd,player,exppattern)
	if exppattern:match(".*#.*") then return false end
	local subpatternlist=exppattern:split("|")
	local b
	local subsubpatternlist
	b=false
	subsubpatternlist=subpatternlist[1]:split(",")
	for _,pattern in pairs(subsubpatternlist) do
		if pattern=="." then b=true break end
		local positive=true
		if pattern:match("^%^") then positive=false pattern=pattern:sub(2) end 
		if cd:isKindOf(pattern) then b=positive else b=not positive end
		if b then break end
	end
	if not b then return false end
	if #subpatternlist==1 then return true end
	b=false
	subsubpatternlist=subpatternlist[2]:split(",")
	local suitstr=cd:getSuitString()
	for _,pattern in pairs(subsubpatternlist) do
		if pattern=="." then b=true break end
		local positive=true
		if pattern:match("^%^") then positive=false pattern=pattern:sub(2) end
		if suitstr==pattern then b=positive else b=not positive end
		if b then break end
	end
	if not b then return false end
	if #subpatternlist==2 then return true end
	b=false
	subsubpatternlist=subpatternlist[3]:split(",")
	local num=cd:getNumber()
	for _,pattern in pairs(subsubpatternlist) do
		if pattern=="." then b=true break end
		local tlist=pattern:split("~")
		local from,to
		from=tonumber(tlist[1])
		if #tlist==1 then to=from else to=tonumber(tlist[2]) end
		if num>=from and num<=to then b=true break end
	end
	if not b then return false end
	if #subpatternlist==3 then return true end
	b=false
	subsubpatternlist=subpatternlist[4]:split(",")
	for _,pattern in pairs(subsubpatternlist) do
		if pattern=="." then b=true break end
		if pattern=="hand" and cd:getEffectiveId()>=0 and not player:hasEquip(cd) then b=true break end
		if pattern=="equipped" and player:hasEquip(cd) then b=true break end
	end
	if not b then return false end
	if #subpatternlist==4 then return true end
	b=false
	subsubpatternlist=subpatternlist[5]:split(",")
	for _,pattern in pairs(subsubpatternlist) do
		if pattern=="." then b=true break end
		if pattern=="red" and cd:isRed() then b=true break end
		if pattern=="black" and cd:isBlack() then b=true break end
		if pattern=="colorless" and cd:getSuit()==sgs.Card_NoSuit then b=true break end
	end
	if not b then return false end
	if #subpatternlist==5 then return true end
end
function mymatch(cd,player,exppattern)
	local patternlist=exppattern:split("#")
	for _,pattern in pairs(patternlist) do
		if mymatchone(cd,player,pattern) then return true end
	end
	return false
end
function pattern2judgepattern(getpattern)
	if getpattern:match("(.*):(.*):(.*)") then return getpattern end
	if getpattern:match(".*#.*") then return "(.*):(.*):(.*)" end
	local subpatternlist=getpattern:split("|")
	local suitstrlist={}	
	local numstrlist={}
	if #subpatternlist>2 then
		local suitlist={spade=0,club=0,heart=0,diamond=0}
		for _,suit in pairs(subpatternlist[2]:split(",")) do
			local tlist={}
			if suit:sub(1,1)=="^" then 
				suit=suit:sub(2)				
				for tsuit,tv in pairs(suitlist) do
					if tsuit~=suit and tv==0 then table.insert(tlist,tsuit) end
				end
				
			else
				if suitlist[suit] and suitlist[suit]==0 then
					table.insert(tlist,suit)
				end
			end
			for _,tsuit in pairs(tlist) do
				suitlist[tsuit]=1
				table.insert(suitstrlist,tsuit)
			end
		end
	end
	if #subpatternlist>3 then
		local numlist={0,0,0,0,0,0,0,0,0,0,0,0,0}
		local numstr4jqka="A23456789-JQK"
		for _,num in pairs(subpatternlist[3]:split(",")) do
			local tlist=num:split("~")
			local from,to
			from=tonumber(tlist[1])
			if #tlist==1 then to=from else to=tonumber(tlist[2]) end
			for var=from,to,1 do
				if numlist[var]==0 then
					numlist[var]=1
					if var==10 then table.insert(numstrlist,var)
					else table.insert(numstrlist,numstr4jqka:sub(var,var)) end
				end
			end
		end
	end
	local suitstr=".*"
	local numstr=".*"
	if #suitstrlist>0 then suitstr=table.concat(suitstrlist,"|") end
	if #numstrlist>0 then numstr=table.concat(numstrlist,"|") end
	return "(.*):("..suitstr.."):("..numstr..")"
end
function myjudgestruct(getpattern,getwho,getreason,getgood)
	local judge=sgs.JudgeStruct()
	judge.pattern=sgs.QRegExp(pattern2judgepattern(getpattern))
	judge.who=getwho
	judge.reason=getreason
	judge.good=getgood or true
	return judge
end

function addsk(sk)
if not sgs.Sanguosha:getSkill(sk:objectName()) then
local sklist=sgs.SkillList()
sklist:append(sk)
sgs.Sanguosha:addSkills(sklist)
end
end
function addexsk(gn,skname)
if sgs.Sanguosha:getSkill(skname) then
gn:addSkill(skname)
for _,tsk in sgs.qlist(sgs.Sanguosha:getRelatedSkills(skname)) do
gn:addSkill(tsk:objectName())
end
end
end

function addsktrans(t,name,trans,dscrpt,wds)
t[name]=trans
t[":"..name]=dscrpt
if wds then
if #wds==1 then t["$"..name]=wds[1]
else table.foreach(wds,function(i,v) t[string.format("$%s%d",name,i)]=v end)
end
end
end
function addgtrans(t,name,trans,title,wdf,cv)
t[name]=trans
t["#"..name]=title
t["~"..name]=wdf
t["cv:"..name]=cv
end
function cmpltrans(t)
local key,value
local tmplist4here={}
for key,value in pairs(t) do
tmplist4here[key]=value
if string.sub(key,1,1)==":" and not t["~"..string.sub(key,2)] then
tmplist4here["~"..string.sub(key,2)]=value
end
if string.sub(key,1,1)==":" and not t["@"..string.sub(key,2)] then
tmplist4here["@"..string.sub(key,2)]=t[string.sub(key,2)]
end
if string.sub(key,1,1)~=":" and not t[":"..key] then
tmplist4here["designer:"..key]=tmplist4here["designer:"..key] or ""
tmplist4here["illustrator:"..key]=tmplist4here["illustrator:"..key] or "Internet"
end
end
return tmplist4here
end