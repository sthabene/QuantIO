function dayCount (Day1, Day2, Month1, Month2, Year1, Year2)
	if Day1 == 31 then Day1 = 30 end
	return 360 * (Year2 - Year1) + 30 * (Month2 - Month1) + (Day2 - Day1)
end
print(dayCount(30, 30, 1, 1, 2021, 2023))