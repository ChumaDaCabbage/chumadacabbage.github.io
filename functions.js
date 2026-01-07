// function isElementOnScreen(element) 
// {
//   const rect = element.getBoundingClientRect();
//   const windowHeight = window.innerHeight || document.documentElement.clientHeight;
//   const windowWidth = window.innerWidth || document.documentElement.clientWidth;

//   return (
//     rect.top < windowHeight && rect.bottom > 0 && //Element is vertically within the viewport
//     rect.left < windowWidth && rect.right > 0    //Element is horizontally within the viewport
//   );
// }
// //Function to scroll to contact section
// function scrollToFooter() 
// {
//   const footerElement = document.querySelector('.footer');
//   footerElement.scrollIntoView({ behavior: 'smooth' });

//   //Highlight the text elements in the footer
//   const textElements = document.querySelectorAll('.footer .contactInfo .item');
  
//   if(isElementOnScreen(footerElement))
//   {
//     textElements.forEach((textElement) => 
//     {
//       textElement.classList.add('highlightShake');
      
//       //Remove the highlight class after a delay (1 second)
//       setTimeout(() => {textElement.classList.remove('highlightShake');}, 1000);
//     });
//   }
//   else
//   {
//     textElements.forEach((textElement) => 
//     {
//       textElement.classList.add('highlight');
        
//       //Remove the highlight class after a delay (1 second)
//       setTimeout(() => {textElement.classList.remove('highlight');}, 1000);
//     }); 
//   }
// }

// function highlightInfo() 
// {
//   //Highlight the text elements in the footer
//   const textElements = document.querySelectorAll('.item');
  
//   textElements.forEach((textElement) => 
//   {
//     textElement.classList.add('highlight');
    
//     //Remove the highlight class after a delay (1 second)
//     setTimeout(() => {textElement.classList.remove('highlight');}, 1000);
//   });
// }

//Function to load the code from a file
async function loadCode(filename) 
{
  try 
  {
    const response = await fetch(filename);  //The file to fetch
    if (response.ok) 
    {
      const code = await response.text();
      document.getElementById(filename).innerHTML = `<pre class="prettyprint lang-cpp">${code}</pre>`;
      PR.prettyPrint();  //Run Google Prettify to style the code     
      //scrollGrower(filename);
    } 
    else 
    {
      console.error('Failed to load the code file');
    }
  } 
  catch (error) 
  {
    console.error('Error loading the code:', error);
  }
}

//Function to load HTML into a div or at the start/end of the body
async function loadHTML(filename, position = "original") 
{
  try 
  {
    const response = await fetch(filename);  //The file to fetch
    if (!response.ok) 
    {
      console.error('Failed to load the html file');
      return;
    }

    const html = await response.text();

    switch (position) 
    {
      case "start":
        document.body.insertAdjacentHTML('afterbegin', html);
        break;
      case "end":
        document.body.insertAdjacentHTML('beforeend', html);
        break;
      case "original":
      default:
        const container = document.getElementById(filename);
        if (container) 
        {
          container.innerHTML = html;
        } 
        else 
        {
          console.error(`No element with id "${filename}" found.`);
        }
        break;
    }
  } 
  catch (error) 
  {
    console.error('Error loading the html:', error);
  }
}

async function loadPageDefualts(hasFooter = true) 
{
  await loadHTML('/header.html', "start");
  await loadHTML('/globalDefaults.html', "start");
  if (hasFooter) await loadHTML('/footer.html', "end");
}

// //Set y offset based off another elements height
// function setOffsetFromHeight(sourceClass, targetClass) 
// {
//   //Get the height of the source element
//   const sourceElements = document.getElementsByClassName(sourceClass);
//   sourceElement = sourceElements[0]; //Get the first element
//   const targetElements = document.getElementsByClassName(targetClass);
//   targetElement = targetElements[0]; //Get the first element

//   if (sourceElement && targetElement) 
//   {
//     const sourceHeight = sourceElement.offsetHeight; //Get the height of the source
//     targetElement.style.top = `${-sourceHeight - 10}px`;   //Set the top offset of the target
//   } 
//   else 
//   {
//     console.error("One or both elements not found.");
//   }
// }

//Cleans up paths
function normalizePath(path)
{
  return path
    .replace(/index\.html$/, '')
    .replace(/\/+$/, '') || '/';
}

//Decides given a path if a navigation or a reload is needed
function navigateOrReload(path)
{
  const current = normalizePath(window.location.pathname);
  const target  = normalizePath(path);

  //Already there
  if (current === target)
  {
    //Just reload
    window.location.reload();
  }
  else
  {
    window.location.href = path;
  }
}

// let StartHeight = -1; //Starting height of the element

// //Increases size of element by how much scrolling has been done
// function scrollGrower(elementId)
// {
//   //If the starting height has not been set, set it
//   if(StartHeight === -1)
//   {
//     const element = document.getElementById(elementId);
//     StartHeight = element.offsetHeight;

//     //Print the starting height
//     console.log(`Start Height: ${StartHeight}`);
//   }

//   //Start with the distance from the top of this element to the bottom of the screen
//   const element = document.getElementById(elementId);
//   const rect = element.getBoundingClientRect();
//   distance = window.innerHeight - rect.top;

//   //Add how much has been scrolled to the distance in the y
//   const scrollDistance = window.scrollY;
//   distance += scrollDistance;

//   //Subtract the height of the scroll bar
//   const scrollBarHeight = window.innerHeight - document.documentElement.clientHeight;
//   distance -= 20;

//   //Print distance scroll distance and the distance from the top of the element to the bottom of the screen
//   console.log(`Scroll Distance: ${scrollDistance}`);
//   console.log(`Distance: ${distance}`);
//   console.log(`Distance: ${distance}`);

//   //Set the elements height to the new distance
//   element.style.height = `${distance}px`;
// }