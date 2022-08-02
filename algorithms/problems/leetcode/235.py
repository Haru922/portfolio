class Solution:
    def minMeetingRooms(self, intervals: List[List[int]]) -> int:
        meeting_rooms=[]
        intervals.sort(key=lambda x: x[0])
        heapq.heappush(meeting_rooms,intervals[0][1])
        for meeting in intervals[1:]:
            if meeting_rooms[0]<=meeting[0]:
                heapq.heappushpop(meeting_rooms,meeting[1])
            else:
                heapq.heappush(meeting_rooms,meeting[1])
        return len(meeting_rooms)
