import System.Windows.Forms

Application.EnableVisualStyles()

f = Form(Text: "Test")
b = Button(Text: "Choose a color")
b.Click += { ColorDialog().ShowDialog(f) }

f.Controls.Add(b)


Application.Run(f)
