function scrollToFooter() 
{
  const footerElement = document.querySelector('.footer');
  footerElement.scrollIntoView({ behavior: 'smooth' });
  highlightText()
}

function highlightText() 
{
  const textElements = document.querySelectorAll('.footer .contactInfo .text');
  
  textElements.forEach((textElement) => 
  {
    textElement.classList.add('highlight');
    
    //Remove the highlight class after a delay (1 second)
    setTimeout(() => {textElement.classList.remove('highlight');}, 1000);
  });
}