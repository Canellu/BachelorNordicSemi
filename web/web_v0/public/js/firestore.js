var firebaseConfig = {
  apiKey: "AIzaSyAil5xS0TQ6n0asypnjm6FJ34g04HJR83M",
  authDomain: "nordicoasys.firebaseapp.com",
  projectId: "nordicoasys",
  storageBucket: "nordicoasys.appspot.com",
  messagingSenderId: "40698568655",
  appId: "1:40698568655:web:e8cc72d0382fd78ec4fcba",
  measurementId: "G-M2MHDRBW7K"
};
// Initialize Firebase
firebase.initializeApp(firebaseConfig);

var db = firebase.firestore();




const glidersCollection = db.collection('Gliders');


async function getDocs()  {

  const dataG1 = glidersCollection.doc('G1').collection('Data');
  const docs = await dataG1.get();
  docs.forEach(doc => {
    console.log(doc.id, '=>', doc);
  });
}


getDocs();