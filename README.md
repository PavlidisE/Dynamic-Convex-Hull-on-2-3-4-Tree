<h1 align = "center" >
  Dynamic Convex Hull on 2-3-4 Tree
</h1>

<p align = "center">
  <img src = "https://miro.medium.com/max/677/1*F4IUmOJbbLMJiTgHxpoc7Q.png"> 
</p>

This is an implementation of the dynamic convex hull algorithm by Overmars and van Leeuwen as described in [1].

The implementation of this algorithm on this particular data structure, as well as its design and testing data are explored in our paper [2].
We build the necessary concatenable queue data strusture, based on a leaf-oriented 2-3-4 tree.
The basic functions (insertions, deletions, rebalancing) of the 2-3-4 tree follow in the same principles as an ordinary 2-3-4 tree. 

Finally the proposed tree, is based on the same 2-3-4 augmented tree as the concatenable queue, with the addition that it stores the necessary extra information needed to calculate the convex arcs. 

The resulting data structure calculates both the left and right convex arcs and outputs the full convex hull of the set of points.


*[1] M. H. Overmars and J. van Leeuwen. Maintenance of configurations in the plane. J. Comput. Syst. Sci., 23(2):166–204, 1981.* 

*[2] Pavlidis,Evripidis - "Dynamic Convex Hulls in 2-4 Trees", July 8, 2021. (http://hdl.handle.net/10889/14997)*
