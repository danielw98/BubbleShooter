stuff to fix/add:

- add bonuspoints for levels (easy)
- bubbles can always be added, just maybe it gets overfilled
- handle hitting the walls with the bubbles and color effect on bubbles that will break wall
- add sound effects
- add gameover screen
- add startgame screen
- animation when launching ball (the cannon)
- animation when launching ball (the lady)
- animation idle (the lady)
- score animation when breaking bubbles
- bubbles animation when falling
- LOCAL VERSION DONE! (?)

fixed/added:
- left wall bug (notice that the slope of the line drawn switches between negative and positive..)
    *fixnote: just increased startx by 3 pixels
- sometimes the bubbles don't want to get destroyed, although they are 3 or 4. Look into that...
    *fixnote: the neighbouring indices in the matrix were wrong, fixed that by drawing in paint
- the colors in the cannon and spare bubble should only be one of those that exist on the board
    *fixnote: not too hard, just have to consider when im shooting and there's a cannonbubble or
    sparebubble of the color not on the table
- a cluster of bubbles that is isolated after clearing the bubbles holding it should be cleared
    *fixnote: ughh, a bit of a pain, but the algorithm worked flawlessly after it was set up and 
    segmentation faults were dealt with (due to wrong variable names and omitting an important 
    function call if(InPath()==false)...)
- score should be updated accordingly
    *fixnote: kinda tricky computing the scores, but did it after watching multiple videos
- when shooting multiple times, update the bar until 5 consecutive shoots then increase 
score given to 150%
    *fixnote: using a rectangle with negative height to make it grow upwards without changing (x,y)
- fix the velocity so that the bubble travels at a constant speed, no matter if it hits the walls or
if it's launched
    *fixnote: just had to normalize the direction to a length that allows an optimal dt 
    for any ball speed chosen - still have to adjust numbers
- now the levels as well as some of the gamestats are created in files and loaded at runtime
- make the board rotate left/right when hitting bubbles with the right velocity
    *fixnote: still got some work to do, as the origin is in the top left corner and the y axes
    grows downwards, gotta figure the proper angles.