"""My NextPalindrome Module"""

def NextPalindrome(pal):
    """Takes in a 5 digit string describing a Palindrome
    Returns a string with the next 5 digit palindrome.
    
    defaults to "99999" if wrong value is given
    """
    nextPal = "99999" #defualt return value
    
    if int(pal[2]) < 9: #checks if the middle digit is 9
        midPal = str(int(pal[2]) + 1) #update middle digit
        nextPal = pal[:2] + midPal + pal[3:]
        
    elif int(pal[1]) < 9: #checks if the 2nd and 4th digit are 9s
        midPal = str(0) #overflow of middle digit
        leftMidPal = str(int(pal[1]) + 1)
        rightMidPal = leftMidPal
        nextPal = pal[0]+leftMidPal+midPal+rightMidPal+pal[-1] 
        
    elif int(pal[0]) < 9: #checks if the 1st and last digits are 9s
        midPal = str(0) #overflow of middle digit
        leftMidPal = str(0) #overflow of 2nd and 4th digit
        leftPal = str(int(pal[0]) + 1)
        rightMidPal = leftMidPal
        rightPal = leftPal
        nextPal = leftPal+leftMidPal+midPal+rightMidPal+rightPal
        
    return nextPal

#--functionality_0
#--Passed all tests!
#--END

#--design_0
#--Well designed!
#--END

#--style_0
#--Good coding style!
#--END

